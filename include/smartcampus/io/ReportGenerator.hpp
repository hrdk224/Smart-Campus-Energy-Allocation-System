#ifndef SMARTCAMPUS_REPORT_GENERATOR_HPP
#define SMARTCAMPUS_REPORT_GENERATOR_HPP

#include "smartcampus/engine/AllocationRecord.hpp"
#include <vector>
#include <string>

namespace smartcampus {

/**
 * @brief Generates formatted reports to terminal output and disk files.
 * Demonstrates:
 * - File I/O (ofstream text and CSV export)
 * - Using generic template utility MetricAggregator
 * - Separation of presentation concerns from business logic
 */
class ReportGenerator {
public:
    /**
     * @brief Formats a complete multi-slot simulation summary as a string.
     */
    static std::string generateSummaryText(const std::vector<AllocationRecord>& history,
                                           const std::string& campusName);

    /**
     * @brief Saves simulation history to a human-readable text file.
     * @return Path of saved file
     */
    static std::string saveReportToFile(const std::vector<AllocationRecord>& history,
                                        const std::string& campusName,
                                        const std::string& outputDirectory = "reports");

    /**
     * @brief Exports simulation history to a CSV file for spreadsheet analysis.
     * @return Path of saved CSV file
     */
    static std::string exportToCsv(const std::vector<AllocationRecord>& history,
                                   const std::string& outputDirectory = "reports");
};

} // namespace smartcampus

#endif // SMARTCAMPUS_REPORT_GENERATOR_HPP
