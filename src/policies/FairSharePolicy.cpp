#include "smartcampus/policies/FairSharePolicy.hpp"
#include <algorithm>
#include <cmath>

namespace smartcampus {

std::string FairSharePolicy::getPolicyName() const {
    return "Fair-Share Proportional Policy";
}

std::string FairSharePolicy::getDescription() const {
    return "Guarantees essential baselines, then distributes remaining discretionary supply "
           "proportionally among all buildings according to their requested loads, preventing starvation. "
           "In severe shortages, essential loads are scaled proportionally.";
}

std::map<std::string, EnergyAllocation> FairSharePolicy::allocate(
    const std::vector<Building*>& buildings,
    double availableSupplyKwh) {

    std::map<std::string, EnergyAllocation> resultMap;

    // Initialize allocations
    for (const auto* b : buildings) {
        EnergyAllocation alloc;
        alloc.requestedKwh = b->getTotalDemand();
        alloc.essentialRequestedKwh = b->getEssentialDemand();
        alloc.discretionaryRequestedKwh = b->getDiscretionaryDemand();
        alloc.allocatedKwh = 0.0;
        alloc.essentialAllocatedKwh = 0.0;
        alloc.discretionaryAllocatedKwh = 0.0;
        alloc.unmetKwh = alloc.requestedKwh;
        alloc.status = FulfillmentStatus::Deferred;
        resultMap[b->getId()] = alloc;
    }

    double remainingSupply = std::max(0.0, availableSupplyKwh);

    double totalEssentialDemand = 0.0;
    double totalDiscretionaryDemand = 0.0;
    for (const auto* b : buildings) {
        totalEssentialDemand += b->getEssentialDemand();
        totalDiscretionaryDemand += b->getDiscretionaryDemand();
    }

    // Stage 1: Essential Allocations
    if (remainingSupply >= totalEssentialDemand) {
        // Satisfy 100% of essential demands
        for (const auto* b : buildings) {
            auto& alloc = resultMap[b->getId()];
            alloc.essentialAllocatedKwh = b->getEssentialDemand();
            alloc.allocatedKwh += alloc.essentialAllocatedKwh;
            remainingSupply -= alloc.essentialAllocatedKwh;
        }
    } else {
        // Severe Shortage: Proportional reduction of essential demands
        if (totalEssentialDemand > 1e-4) {
            double supplyRatio = remainingSupply / totalEssentialDemand;
            double distributed = 0.0;
            for (const auto* b : buildings) {
                auto& alloc = resultMap[b->getId()];
                double share = b->getEssentialDemand() * supplyRatio;
                alloc.essentialAllocatedKwh = share;
                alloc.allocatedKwh += share;
                distributed += share;
                alloc.note = "Emergency: Proportional essential load reduction.";
            }
            remainingSupply = std::max(0.0, remainingSupply - distributed);
        }
    }

    // Stage 2: Discretionary Allocations (Proportional Fair Share)
    if (remainingSupply > 1e-4 && totalDiscretionaryDemand > 1e-4) {
        if (remainingSupply >= totalDiscretionaryDemand) {
            // Full discretionary supply available
            for (const auto* b : buildings) {
                auto& alloc = resultMap[b->getId()];
                double discDemand = b->getDiscretionaryDemand();
                alloc.discretionaryAllocatedKwh = discDemand;
                alloc.allocatedKwh += discDemand;
            }
        } else {
            // Distribute proportionally to discretionary demand
            double shareRatio = remainingSupply / totalDiscretionaryDemand;
            for (const auto* b : buildings) {
                auto& alloc = resultMap[b->getId()];
                double share = b->getDiscretionaryDemand() * shareRatio;
                alloc.discretionaryAllocatedKwh = share;
                alloc.allocatedKwh += share;
            }
        }
    }

    // Finalize status and unmet demand
    for (const auto* b : buildings) {
        auto& alloc = resultMap[b->getId()];
        alloc.unmetKwh = std::max(0.0, alloc.requestedKwh - alloc.allocatedKwh);

        if (alloc.unmetKwh <= 1e-4) {
            alloc.status = FulfillmentStatus::FullyMet;
            if (alloc.note.empty()) alloc.note = "100% Demand satisfied.";
        } else if (alloc.allocatedKwh > 1e-4) {
            alloc.status = FulfillmentStatus::PartiallyMet;
            if (alloc.note.empty()) alloc.note = "Equitable proportional share allocated.";
        } else {
            alloc.status = FulfillmentStatus::Deferred;
            if (alloc.note.empty()) alloc.note = "Deferred due to zero available discretionary power.";
        }
    }

    return resultMap;
}

} // namespace smartcampus
