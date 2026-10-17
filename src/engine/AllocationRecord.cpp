#include "smartcampus/engine/AllocationRecord.hpp"
#include <iomanip>

namespace smartcampus {

AllocationRecord::AllocationRecord(TimeSlot slot,
                                   std::string policyName,
                                   EnergySupplySummary supplySummary,
                                   std::vector<BuildingAllocationSnapshot> snapshots,
                                   std::vector<std::string> warnings)
    : m_timeSlot(std::move(slot)),
      m_policyName(std::move(policyName)),
      m_supplySummary(supplySummary),
      m_snapshots(std::move(snapshots)),
      m_warnings(std::move(warnings)),
      m_totalDemandKwh(0.0),
      m_totalEssentialKwh(0.0),
      m_totalDiscretionaryKwh(0.0),
      m_totalAllocatedKwh(0.0),
      m_totalUnmetKwh(0.0) {

    for (const auto& s : m_snapshots) {
        m_totalDemandKwh += s.allocation.requestedKwh;
        m_totalEssentialKwh += s.allocation.essentialRequestedKwh;
        m_totalDiscretionaryKwh += s.allocation.discretionaryRequestedKwh;
        m_totalAllocatedKwh += s.allocation.allocatedKwh;
        m_totalUnmetKwh += s.allocation.unmetKwh;
    }
}

std::ostream& operator<<(std::ostream& os, const AllocationRecord& record) {
    os << "========================================================================================\n";
    os << " SIMULATION REPORT: " << record.m_timeSlot << "\n";
    os << " Policy Applied:    " << record.m_policyName << "\n";
    os << "========================================================================================\n";

    // Supply Summary
    const auto& s = record.m_supplySummary;
    os << "ENERGY DISPATCH BREAKDOWN:\n"
       << std::fixed << std::setprecision(1)
       << "  * Solar Photovoltaic:  Available = " << s.solarAvailableKwh << " kWh | Dispatched = " << s.solarUsedKwh << " kWh\n"
       << "  * Battery Storage:     Available = " << s.batteryAvailableKwh << " kWh | Discharged = " << s.batteryDischargedKwh
       << " kWh | Charged = " << s.batteryChargedKwh << " kWh\n"
       << "                         SoC: " << s.batteryStartSoCKwh << " kWh -> " << s.batteryEndSoCKwh << " kWh\n"
       << "  * Power Grid Backup:   Available = " << s.gridAvailableKwh << " kWh | Dispatched = " << s.gridUsedKwh << " kWh\n"
       << "  * TOTAL ENERGY SUPPLIED: " << s.totalSuppliedKwh() << " kWh (Available: " << s.totalAvailableKwh() << " kWh)\n";

    // Warnings
    if (!record.m_warnings.empty()) {
        os << "\nWARNINGS & NOTICES:\n";
        for (const auto& w : record.m_warnings) {
            os << "  [!] " << w << "\n";
        }
    }

    // Per-building table
    os << "\nBUILDING ALLOCATION BREAKDOWN:\n";
    os << "----------------------------------------------------------------------------------------------------\n";
    os << std::left
       << std::setw(8)  << "ID"
       << std::setw(24) << "Building Name"
       << std::setw(12) << "Priority"
       << std::right
       << std::setw(10) << "Demand"
       << std::setw(10) << "Essential"
       << std::setw(10) << "Allocated"
       << std::setw(10) << "Unmet"
       << std::setw(16) << "Status"
       << "\n";
    os << "----------------------------------------------------------------------------------------------------\n";

    for (const auto& snap : record.m_snapshots) {
        os << std::left
           << std::setw(8)  << snap.buildingId
           << std::setw(24) << snap.buildingName.substr(0, 23)
           << std::setw(12) << priorityToString(snap.priority)
           << std::right << std::fixed << std::setprecision(1)
           << std::setw(10) << snap.allocation.requestedKwh
           << std::setw(10) << snap.allocation.essentialRequestedKwh
           << std::setw(10) << snap.allocation.allocatedKwh
           << std::setw(10) << snap.allocation.unmetKwh
           << std::setw(16) << fulfillmentToString(snap.allocation.status)
           << "\n";
    }

    os << "----------------------------------------------------------------------------------------------------\n";
    os << std::left
       << std::setw(44) << "CAMPUS TOTALS:"
       << std::right << std::fixed << std::setprecision(1)
       << std::setw(10) << record.m_totalDemandKwh
       << std::setw(10) << record.m_totalEssentialKwh
       << std::setw(10) << record.m_totalAllocatedKwh
       << std::setw(10) << record.m_totalUnmetKwh
       << std::setw(14) << (std::to_string(static_cast<int>(record.getFulfillmentPercentage())) + "% met")
       << "\n";
    os << "========================================================================================\n";

    return os;
}

} // namespace smartcampus
