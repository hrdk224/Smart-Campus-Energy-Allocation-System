#include "smartcampus/io/ReportGenerator.hpp"
#include "smartcampus/utils/GenericUtils.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>

namespace smartcampus {

std::string ReportGenerator::generateSummaryText(const std::vector<AllocationRecord>& history,
                                                 const std::string& campusName) {
    std::ostringstream oss;
    oss << "========================================================================================\n";
    oss << " SMART CAMPUS ENERGY ALLOCATION SYSTEM - EXECUTIVE SUMMARY REPORT\n";
    oss << " Campus Name: " << campusName << "\n";
    oss << " Total Time Slots Simulated: " << history.size() << "\n";
    oss << "========================================================================================\n";

    if (history.empty()) {
        oss << "No simulation records available.\n";
        return oss.str();
    }

    // Use MetricAggregator template to accumulate metrics
    utils::MetricAggregator<double> demandAggregator;
    utils::MetricAggregator<double> allocatedAggregator;
    utils::MetricAggregator<double> unmetAggregator;
    utils::MetricAggregator<double> solarUsedAggregator;
    utils::MetricAggregator<double> gridUsedAggregator;
    utils::MetricAggregator<double> batteryThroughputAggregator;

    for (const auto& r : history) {
        demandAggregator.addSample(r.getTotalDemand());
        allocatedAggregator.addSample(r.getTotalAllocated());
        unmetAggregator.addSample(r.getTotalUnmet());
        solarUsedAggregator.addSample(r.getSupplySummary().solarUsedKwh);
        gridUsedAggregator.addSample(r.getSupplySummary().gridUsedKwh);
        batteryThroughputAggregator.addSample(r.getSupplySummary().batteryDischargedKwh);
    }

    oss << std::fixed << std::setprecision(1);
    oss << "CAMPUS-WIDE AGGREGATE ENERGY METRICS:\n";
    oss << "  * Total Campus Demand Requested: " << demandAggregator.sum() << " kWh (Avg: "
        << demandAggregator.average() << " kWh/slot)\n";
    oss << "  * Total Energy Allocated:        " << allocatedAggregator.sum() << " kWh (Avg: "
        << allocatedAggregator.average() << " kWh/slot)\n";
    oss << "  * Total Demand Deferred/Unmet:   " << unmetAggregator.sum() << " kWh\n";

    double overallFulfillment = (demandAggregator.sum() > 1e-4)
        ? (allocatedAggregator.sum() / demandAggregator.sum()) * 100.0 : 100.0;
    oss << "  * Overall Campus Fulfillment:    " << overallFulfillment << "%\n";

    oss << "\nSOURCE GENERATION BREAKDOWN:\n";
    oss << "  * Clean Solar Energy Dispatched: " << solarUsedAggregator.sum() << " kWh ("
        << (allocatedAggregator.sum() > 1e-4 ? (solarUsedAggregator.sum() / allocatedAggregator.sum()) * 100.0 : 0.0)
        << "% of total consumed)\n";
    oss << "  * Battery Storage Discharged:    " << batteryThroughputAggregator.sum() << " kWh\n";
    oss << "  * Power Grid Energy Imported:    " << gridUsedAggregator.sum() << " kWh\n";

    oss << "\nTIME-SLOT SUMMARY TABLE:\n";
    oss << "----------------------------------------------------------------------------------------\n";
    oss << std::left
        << std::setw(10) << "Slot #"
        << std::setw(28) << "Policy"
        << std::right
        << std::setw(10) << "Demand"
        << std::setw(10) << "Alloc"
        << std::setw(10) << "Unmet"
        << std::setw(10) << "Solar"
        << std::setw(10) << "Grid"
        << "\n";
    oss << "----------------------------------------------------------------------------------------\n";

    for (const auto& r : history) {
        oss << std::left
            << std::setw(10) << ("Slot " + std::to_string(r.getTimeSlot().getSlotNumber()))
            << std::setw(28) << r.getPolicyName().substr(0, 27)
            << std::right
            << std::setw(10) << r.getTotalDemand()
            << std::setw(10) << r.getTotalAllocated()
            << std::setw(10) << r.getTotalUnmet()
            << std::setw(10) << r.getSupplySummary().solarUsedKwh
            << std::setw(10) << r.getSupplySummary().gridUsedKwh
            << "\n";
    }
    oss << "========================================================================================\n";

    return oss.str();
}

std::string ReportGenerator::saveReportToFile(const std::vector<AllocationRecord>& history,
                                             const std::string& campusName,
                                             const std::string& outputDirectory) {
    std::string filePath = outputDirectory + "/campus_energy_report.txt";
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        throw std::runtime_error("Could not write report to file: " + filePath);
    }

    outFile << generateSummaryText(history, campusName) << "\n\n";

    outFile << "DETAILED PER-SLOT BREAKDOWN:\n\n";
    for (const auto& r : history) {
        outFile << r << "\n";
    }

    outFile.close();
    return filePath;
}

std::string ReportGenerator::exportToCsv(const std::vector<AllocationRecord>& history,
                                        const std::string& outputDirectory) {
    std::string filePath = outputDirectory + "/simulation_results.csv";
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        throw std::runtime_error("Could not write CSV to file: " + filePath);
    }

    outFile << "SlotNumber,TimeRange,Policy,BuildingID,BuildingName,Category,Priority,"
            << "EssentialDemand,DiscretionaryDemand,TotalDemand,AllocatedKwh,UnmetKwh,Status,Note\n";

    for (const auto& r : history) {
        for (const auto& s : r.getSnapshots()) {
            outFile << r.getTimeSlot().getSlotNumber() << ",\""
                    << r.getTimeSlot().getTimeRange() << "\",\""
                    << r.getPolicyName() << "\",\""
                    << s.buildingId << "\",\""
                    << s.buildingName << "\",\""
                    << categoryToString(s.category) << "\",\""
                    << priorityToString(s.priority) << "\","
                    << std::fixed << std::setprecision(1)
                    << s.allocation.essentialRequestedKwh << ","
                    << s.allocation.discretionaryRequestedKwh << ","
                    << s.allocation.requestedKwh << ","
                    << s.allocation.allocatedKwh << ","
                    << s.allocation.unmetKwh << ",\""
                    << fulfillmentToString(s.allocation.status) << "\",\""
                    << s.allocation.note << "\"\n";
        }
    }

    outFile.close();
    return filePath;
}

} // namespace smartcampus
