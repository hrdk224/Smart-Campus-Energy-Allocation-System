#include "smartcampus/common/Types.hpp"
#include <sstream>
#include <cmath>

namespace smartcampus {

std::string categoryToString(BuildingCategory category) {
    switch (category) {
        case BuildingCategory::Academic: return "Academic";
        case BuildingCategory::Residential: return "Residential";
        case BuildingCategory::Administrative: return "Administrative";
        case BuildingCategory::SpecializedLab: return "SpecializedLab";
        default: return "Unknown";
    }
}

std::string priorityToString(PriorityLevel priority) {
    switch (priority) {
        case PriorityLevel::Critical: return "P1-Critical";
        case PriorityLevel::High:     return "P2-High";
        case PriorityLevel::Medium:   return "P3-Medium";
        case PriorityLevel::Low:      return "P4-Low";
        default:                      return "Unknown";
    }
}

std::string fulfillmentToString(FulfillmentStatus status) {
    switch (status) {
        case FulfillmentStatus::FullyMet:     return "FULLY MET";
        case FulfillmentStatus::PartiallyMet: return "PARTIALLY MET";
        case FulfillmentStatus::Deferred:     return "DEFERRED";
        default:                              return "UNKNOWN";
    }
}

// TimeSlot implementation
TimeSlot::TimeSlot()
    : m_slotNumber(1), m_timeRange("08:00 - 10:00"), m_description("Morning Slot") {}

TimeSlot::TimeSlot(int slotNumber, std::string timeRange, std::string description)
    : m_slotNumber(slotNumber),
      m_timeRange(std::move(timeRange)),
      m_description(std::move(description)) {}

bool TimeSlot::operator==(const TimeSlot& other) const {
    return m_slotNumber == other.m_slotNumber && m_timeRange == other.m_timeRange;
}

bool TimeSlot::operator<(const TimeSlot& other) const {
    return m_slotNumber < other.m_slotNumber;
}

std::ostream& operator<<(std::ostream& os, const TimeSlot& slot) {
    os << "Slot #" << slot.m_slotNumber << " [" << slot.m_timeRange << "] "
       << (slot.m_description.empty() ? "" : "(" + slot.m_description + ")");
    return os;
}

// EnergyAllocation implementation
EnergyAllocation EnergyAllocation::operator+(const EnergyAllocation& other) const {
    EnergyAllocation res;
    res.requestedKwh = this->requestedKwh + other.requestedKwh;
    res.essentialRequestedKwh = this->essentialRequestedKwh + other.essentialRequestedKwh;
    res.discretionaryRequestedKwh = this->discretionaryRequestedKwh + other.discretionaryRequestedKwh;

    res.allocatedKwh = this->allocatedKwh + other.allocatedKwh;
    res.essentialAllocatedKwh = this->essentialAllocatedKwh + other.essentialAllocatedKwh;
    res.discretionaryAllocatedKwh = this->discretionaryAllocatedKwh + other.discretionaryAllocatedKwh;

    res.unmetKwh = this->unmetKwh + other.unmetKwh;

    if (res.unmetKwh <= 1e-4) {
        res.status = FulfillmentStatus::FullyMet;
    } else if (res.allocatedKwh > 1e-4) {
        res.status = FulfillmentStatus::PartiallyMet;
    } else {
        res.status = FulfillmentStatus::Deferred;
    }
    return res;
}

EnergyAllocation& EnergyAllocation::operator+=(const EnergyAllocation& other) {
    *this = *this + other;
    return *this;
}

bool EnergyAllocation::operator==(const EnergyAllocation& other) const {
    return std::fabs(allocatedKwh - other.allocatedKwh) < 1e-4 &&
           std::fabs(unmetKwh - other.unmetKwh) < 1e-4 &&
           status == other.status;
}

std::ostream& operator<<(std::ostream& os, const EnergyAllocation& alloc) {
    os << std::fixed << std::setprecision(1)
       << "Req: " << alloc.requestedKwh << " kWh (Ess: " << alloc.essentialRequestedKwh
       << ") | Alloc: " << alloc.allocatedKwh << " kWh | Unmet: " << alloc.unmetKwh
       << " kWh | Status: [" << fulfillmentToString(alloc.status) << "]";
    return os;
}

} // namespace smartcampus
