#ifndef SMARTCAMPUS_ALLOCATION_RECORD_HPP
#define SMARTCAMPUS_ALLOCATION_RECORD_HPP

#include "smartcampus/common/Types.hpp"
#include <map>
#include <vector>
#include <string>
#include <iostream>

namespace smartcampus {

/**
 * @brief Individual building snapshot inside an allocation record.
 */
struct BuildingAllocationSnapshot {
    std::string buildingId;
    std::string buildingName;
    std::string typeName;
    BuildingCategory category;
    PriorityLevel priority;
    EnergyAllocation allocation;
};

/**
 * @brief Comprehensive audit record of an energy allocation run for one time slot.
 * Demonstrates:
 * - Composition of domain types
 * - Audit logging and invariant recording
 * - Operator overloading for reporting
 */
class AllocationRecord {
private:
    TimeSlot m_timeSlot;
    std::string m_policyName;
    EnergySupplySummary m_supplySummary;
    std::vector<BuildingAllocationSnapshot> m_snapshots;
    std::vector<std::string> m_warnings;

    double m_totalDemandKwh;
    double m_totalEssentialKwh;
    double m_totalDiscretionaryKwh;
    double m_totalAllocatedKwh;
    double m_totalUnmetKwh;

public:
    AllocationRecord(TimeSlot slot,
                     std::string policyName,
                     EnergySupplySummary supplySummary,
                     std::vector<BuildingAllocationSnapshot> snapshots,
                     std::vector<std::string> warnings);

    const TimeSlot& getTimeSlot() const { return m_timeSlot; }
    const std::string& getPolicyName() const { return m_policyName; }
    const EnergySupplySummary& getSupplySummary() const { return m_supplySummary; }
    const std::vector<BuildingAllocationSnapshot>& getSnapshots() const { return m_snapshots; }
    const std::vector<std::string>& getWarnings() const { return m_warnings; }

    double getTotalDemand() const { return m_totalDemandKwh; }
    double getTotalEssential() const { return m_totalEssentialKwh; }
    double getTotalDiscretionary() const { return m_totalDiscretionaryKwh; }
    double getTotalAllocated() const { return m_totalAllocatedKwh; }
    double getTotalUnmet() const { return m_totalUnmetKwh; }

    double getFulfillmentPercentage() const {
        return (m_totalDemandKwh > 1e-4) ? (m_totalAllocatedKwh / m_totalDemandKwh) * 100.0 : 100.0;
    }

    friend std::ostream& operator<<(std::ostream& os, const AllocationRecord& record);
};

} // namespace smartcampus

#endif // SMARTCAMPUS_ALLOCATION_RECORD_HPP
