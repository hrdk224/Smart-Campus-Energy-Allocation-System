#include "smartcampus/models/Building.hpp"
#include "smartcampus/common/Exceptions.hpp"
#include <iomanip>
#include <cmath>

namespace smartcampus {

// Initialize static member
int Building::s_activeBuildingCount = 0;

Building::Building(std::string id,
                   std::string name,
                   BuildingCategory category,
                   PriorityLevel priority,
                   double essentialDemand,
                   double discretionaryDemand)
    : m_id(std::move(id)),
      m_name(std::move(name)),
      m_category(category),
      m_priority(priority),
      m_essentialDemand(0.0),
      m_discretionaryDemand(0.0),
      m_allocatedEnergy(0.0) {
    setDemands(essentialDemand, discretionaryDemand);
    ++s_activeBuildingCount;
}

Building::~Building() {
    --s_activeBuildingCount;
}

void Building::setDemands(double essential, double discretionary) {
    if (essential < 0.0) {
        throw NegativeEnergyException("Building Essential Demand", essential);
    }
    if (discretionary < 0.0) {
        throw NegativeEnergyException("Building Discretionary Demand", discretionary);
    }
    m_essentialDemand = essential;
    m_discretionaryDemand = discretionary;
}

void Building::setAllocatedEnergy(double kwh) {
    if (kwh < 0.0) {
        throw NegativeEnergyException("Allocated Energy", kwh);
    }
    m_allocatedEnergy = kwh;
}

double Building::getUnmetDemand() const {
    double total = getTotalDemand();
    if (m_allocatedEnergy >= total) {
        return 0.0;
    }
    return total - m_allocatedEnergy;
}

FulfillmentStatus Building::getFulfillmentStatus() const {
    double total = getTotalDemand();
    if (total <= 1e-4) {
        return FulfillmentStatus::FullyMet;
    }
    if (m_allocatedEnergy >= total - 1e-4) {
        return FulfillmentStatus::FullyMet;
    }
    if (m_allocatedEnergy > 1e-4) {
        return FulfillmentStatus::PartiallyMet;
    }
    return FulfillmentStatus::Deferred;
}

EnergyAllocation Building::getAllocationSummary() const {
    EnergyAllocation alloc;
    alloc.requestedKwh = getTotalDemand();
    alloc.essentialRequestedKwh = m_essentialDemand;
    alloc.discretionaryRequestedKwh = m_discretionaryDemand;

    alloc.allocatedKwh = m_allocatedEnergy;
    if (m_allocatedEnergy <= m_essentialDemand) {
        alloc.essentialAllocatedKwh = m_allocatedEnergy;
        alloc.discretionaryAllocatedKwh = 0.0;
    } else {
        alloc.essentialAllocatedKwh = m_essentialDemand;
        alloc.discretionaryAllocatedKwh = m_allocatedEnergy - m_essentialDemand;
    }

    alloc.unmetKwh = getUnmetDemand();
    alloc.status = getFulfillmentStatus();
    return alloc;
}

void Building::resetAllocation() {
    m_allocatedEnergy = 0.0;
}

std::ostream& operator<<(std::ostream& os, const Building& b) {
    os << "[" << b.m_id << "] " << b.m_name << " (" << b.getTypeName() << ") - "
       << priorityToString(b.m_priority) << " | Dem: " << b.getTotalDemand()
       << " kWh (Ess: " << b.m_essentialDemand << ", Disc: " << b.m_discretionaryDemand
       << ") | Alloc: " << b.m_allocatedEnergy << " kWh ["
       << fulfillmentToString(b.getFulfillmentStatus()) << "]";
    return os;
}

} // namespace smartcampus
