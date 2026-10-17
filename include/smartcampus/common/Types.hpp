#ifndef SMARTCAMPUS_TYPES_HPP
#define SMARTCAMPUS_TYPES_HPP

#include <string>
#include <iostream>
#include <iomanip>

namespace smartcampus {

/**
 * @brief Categorization of campus buildings reflecting different functional roles.
 */
enum class BuildingCategory {
    Academic,
    Residential,
    Administrative,
    SpecializedLab
};

/**
 * @brief Priority levels for energy distribution.
 * Lower numerical value denotes strictly higher priority.
 */
enum class PriorityLevel {
    Critical = 1,  // Labs, health center, data server rooms
    High = 2,      // Major academic lecture halls, library
    Medium = 3,    // Hostels, dining halls
    Low = 4        // Non-essential administrative offices, sports complex
};

/**
 * @brief Status of energy allocation for a specific building.
 */
enum class FulfillmentStatus {
    FullyMet,      // Allocated == Requested
    PartiallyMet,  // 0 < Allocated < Requested
    Deferred       // Allocated == 0 (demand > 0)
};

/**
 * @brief Convert BuildingCategory enum to readable string representation.
 */
std::string categoryToString(BuildingCategory category);

/**
 * @brief Convert PriorityLevel enum to readable string representation.
 */
std::string priorityToString(PriorityLevel priority);

/**
 * @brief Convert FulfillmentStatus enum to readable string representation.
 */
std::string fulfillmentToString(FulfillmentStatus status);

/**
 * @brief Represents a discrete simulation time slot (e.g. 08:00 - 10:00).
 */
class TimeSlot {
private:
    int m_slotNumber;
    std::string m_timeRange;
    std::string m_description;

public:
    TimeSlot();
    TimeSlot(int slotNumber, std::string timeRange, std::string description = "");

    int getSlotNumber() const { return m_slotNumber; }
    const std::string& getTimeRange() const { return m_timeRange; }
    const std::string& getDescription() const { return m_description; }

    // Operator overloading for comparison and streaming
    bool operator==(const TimeSlot& other) const;
    bool operator<(const TimeSlot& other) const;
    friend std::ostream& operator<<(std::ostream& os, const TimeSlot& slot);
};

/**
 * @brief Detailed energy allocation result for a single building.
 */
struct EnergyAllocation {
    double requestedKwh{0.0};
    double essentialRequestedKwh{0.0};
    double discretionaryRequestedKwh{0.0};

    double allocatedKwh{0.0};
    double essentialAllocatedKwh{0.0};
    double discretionaryAllocatedKwh{0.0};

    double unmetKwh{0.0};
    FulfillmentStatus status{FulfillmentStatus::Deferred};
    std::string note{""};

    // Operator overloading
    EnergyAllocation operator+(const EnergyAllocation& other) const;
    EnergyAllocation& operator+=(const EnergyAllocation& other);
    bool operator==(const EnergyAllocation& other) const;

    friend std::ostream& operator<<(std::ostream& os, const EnergyAllocation& alloc);
};

/**
 * @brief Summary of campus-wide energy supplies for a time slot.
 */
struct EnergySupplySummary {
    double solarAvailableKwh{0.0};
    double solarUsedKwh{0.0};

    double batteryAvailableKwh{0.0};
    double batteryDischargedKwh{0.0};
    double batteryChargedKwh{0.0};
    double batteryStartSoCKwh{0.0};
    double batteryEndSoCKwh{0.0};

    double gridAvailableKwh{0.0};
    double gridUsedKwh{0.0};

    double totalSuppliedKwh() const {
        return solarUsedKwh + batteryDischargedKwh + gridUsedKwh;
    }
    double totalAvailableKwh() const {
        return solarAvailableKwh + batteryAvailableKwh + gridAvailableKwh;
    }
};

} // namespace smartcampus

#endif // SMARTCAMPUS_TYPES_HPP
