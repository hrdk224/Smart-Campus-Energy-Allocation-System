#include "smartcampus/engine/AllocationEngine.hpp"
#include "smartcampus/common/Exceptions.hpp"
#include <cmath>

namespace smartcampus {

AllocationEngine::AllocationEngine(Campus& campus,
                                   EnergySupplyManager& supplyManager,
                                   std::shared_ptr<AllocationPolicy> initialPolicy)
    : m_campus(campus),
      m_supplyManager(supplyManager),
      m_currentPolicy(std::move(initialPolicy)) {
    if (!m_currentPolicy) {
        throw PolicyException("AllocationEngine requires a valid, non-null AllocationPolicy.");
    }
}

void AllocationEngine::setPolicy(std::shared_ptr<AllocationPolicy> policy) {
    if (!policy) {
        throw PolicyException("Cannot set null AllocationPolicy in engine.");
    }
    m_currentPolicy = std::move(policy);
}

const AllocationPolicy& AllocationEngine::getPolicy() const {
    return *m_currentPolicy;
}

void AllocationEngine::validateInvariants(
    double totalAllocated,
    double availableSupply,
    const std::map<std::string, EnergyAllocation>& allocations) const {

    // Invariant 1: Total allocated must never exceed available supply
    if (totalAllocated > availableSupply + 1e-4) {
        throw SupplyExceededException(totalAllocated, availableSupply);
    }

    // Invariant 2: No allocation may be negative, and cannot exceed requested demand
    for (const auto& pair : allocations) {
        const auto& alloc = pair.second;
        if (alloc.allocatedKwh < -1e-4) {
            throw NegativeEnergyException("Building Allocation for " + pair.first, alloc.allocatedKwh);
        }
        if (alloc.allocatedKwh > alloc.requestedKwh + 1e-4) {
            throw CampusEnergyException("Building " + pair.first + " allocation (" +
                                        std::to_string(alloc.allocatedKwh) +
                                        " kWh) exceeded requested demand (" +
                                        std::to_string(alloc.requestedKwh) + " kWh)");
        }
        if (std::fabs(alloc.unmetKwh - (alloc.requestedKwh - alloc.allocatedKwh)) > 1e-4) {
            throw CampusEnergyException("Building " + pair.first + " unmet demand calculated incorrectly.");
        }
    }
}

AllocationRecord AllocationEngine::simulateTimeSlot(const TimeSlot& slot) {
    m_campus.resetAllocations();
    m_supplyManager.resetForTimeSlot();

    std::vector<Building*> buildings = m_campus.getBuildingPointers();
    double availableSupply = m_supplyManager.getTotalAvailableSupply();
    double totalEssentialDemand = m_campus.getTotalEssentialDemand();
    double totalDemand = m_campus.getTotalDemand();

    // Run allocation policy
    std::map<std::string, EnergyAllocation> allocations =
        m_currentPolicy->allocate(buildings, availableSupply);

    // Compute total allocated by policy
    double totalAllocated = 0.0;
    for (const auto& pair : allocations) {
        totalAllocated += pair.second.allocatedKwh;
    }

    // Invariant validation
    validateInvariants(totalAllocated, availableSupply, allocations);

    // Physically dispatch the allocated total through the supply manager
    // (Also handles routing surplus solar to battery if solar exceeds total campus demand)
    EnergySupplySummary supplySummary = m_supplyManager.dispatchEnergy(
        (availableSupply > totalDemand && m_supplyManager.getSolar().getAvailableEnergy() > totalDemand)
            ? totalDemand
            : totalAllocated
    );

    // Apply allocated energy to each building object
    std::vector<BuildingAllocationSnapshot> snapshots;
    snapshots.reserve(buildings.size());

    for (auto* b : buildings) {
        auto it = allocations.find(b->getId());
        if (it != allocations.end()) {
            b->allocate(it->second.allocatedKwh);

            BuildingAllocationSnapshot snap;
            snap.buildingId = b->getId();
            snap.buildingName = b->getName();
            snap.typeName = b->getTypeName();
            snap.category = b->getCategory();
            snap.priority = b->getPriority();
            snap.allocation = it->second;
            snapshots.push_back(snap);
        }
    }

    // Assemble warnings and notices
    std::vector<std::string> warnings;
    if (availableSupply < totalEssentialDemand) {
        warnings.push_back("CRITICAL SHORTAGE WARNING: Total available supply (" +
                           std::to_string(availableSupply) + " kWh) is BELOW essential baseline demand (" +
                           std::to_string(totalEssentialDemand) + " kWh). Essential services are rationed!");
    } else if (totalAllocated < totalDemand) {
        warnings.push_back("MODERATE SHORTAGE NOTICE: Discretionary demand was rationed under current policy. " +
                           std::to_string(totalDemand - totalAllocated) + " kWh deferred.");
    }

    if (supplySummary.batteryChargedKwh > 0.0) {
        warnings.push_back("SOLAR SURPLUS STORED: " + std::to_string(supplySummary.batteryChargedKwh) +
                           " kWh excess solar routed into battery storage.");
    }

    if (m_supplyManager.getBattery().getSoCPercentage() <= 15.0) {
        warnings.push_back("BATTERY DEPLETION NOTICE: State of charge is at or near minimum health reserve (" +
                           std::to_string(m_supplyManager.getBattery().getSoCPercentage()) + "%).");
    }

    AllocationRecord record(slot,
                           m_currentPolicy->getPolicyName(),
                           supplySummary,
                           std::move(snapshots),
                           std::move(warnings));

    m_simulationHistory.push_back(record);
    return record;
}

} // namespace smartcampus
