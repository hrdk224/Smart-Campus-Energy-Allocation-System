#include "smartcampus/policies/PriorityPolicy.hpp"
#include <algorithm>
#include <cmath>

namespace smartcampus {

std::string PriorityPolicy::getPolicyName() const {
    return "Priority-Based Policy";
}

std::string PriorityPolicy::getDescription() const {
    return "Distributes energy strictly in order of building priority (P1-Critical down to P4-Low). "
           "Essential loads are satisfied first; remaining supply is prioritized for discretionary loads. "
           "Equal-priority ties are broken deterministically by Building ID.";
}

std::map<std::string, EnergyAllocation> PriorityPolicy::allocate(
    const std::vector<Building*>& buildings,
    double availableSupplyKwh) {

    std::map<std::string, EnergyAllocation> resultMap;

    // Create a copy of building pointers sorted by priority ascending (1 = Critical is highest)
    // Tie-break deterministically using Building ID
    std::vector<Building*> sortedBuildings = buildings;
    std::sort(sortedBuildings.begin(), sortedBuildings.end(), [](const Building* a, const Building* b) {
        if (a->getPriority() != b->getPriority()) {
            return static_cast<int>(a->getPriority()) < static_cast<int>(b->getPriority());
        }
        return a->getId() < b->getId();
    });

    // Initialize allocations in the map
    for (const auto* b : sortedBuildings) {
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

    // Calculate total campus essential demand
    double totalEssentialDemand = 0.0;
    for (const auto* b : sortedBuildings) {
        totalEssentialDemand += b->getEssentialDemand();
    }

    // Stage 1: Allocate Essential Demand
    if (remainingSupply >= totalEssentialDemand) {
        // Full essential demand satisfied for all buildings
        for (const auto* b : sortedBuildings) {
            auto& alloc = resultMap[b->getId()];
            alloc.essentialAllocatedKwh = b->getEssentialDemand();
            alloc.allocatedKwh += alloc.essentialAllocatedKwh;
            remainingSupply -= alloc.essentialAllocatedKwh;
        }
    } else {
        // Severe Shortage: Essential demand rationing strictly by priority
        for (const auto* b : sortedBuildings) {
            auto& alloc = resultMap[b->getId()];
            double toGive = std::min(b->getEssentialDemand(), remainingSupply);
            alloc.essentialAllocatedKwh = toGive;
            alloc.allocatedKwh += toGive;
            remainingSupply -= toGive;
            if (toGive < b->getEssentialDemand()) {
                alloc.note = "Emergency: Essential demand rationed due to grid/supply shortfall.";
            }
        }
    }

    // Stage 2: Allocate Discretionary Demand (if remaining supply > 0)
    if (remainingSupply > 1e-4) {
        for (const auto* b : sortedBuildings) {
            if (remainingSupply <= 1e-4) break;
            auto& alloc = resultMap[b->getId()];
            double discDemand = b->getDiscretionaryDemand();
            double toGive = std::min(discDemand, remainingSupply);
            alloc.discretionaryAllocatedKwh = toGive;
            alloc.allocatedKwh += toGive;
            remainingSupply -= toGive;
        }
    }

    // Finalize status and unmet demand
    for (const auto* b : sortedBuildings) {
        auto& alloc = resultMap[b->getId()];
        alloc.unmetKwh = std::max(0.0, alloc.requestedKwh - alloc.allocatedKwh);

        if (alloc.unmetKwh <= 1e-4) {
            alloc.status = FulfillmentStatus::FullyMet;
            if (alloc.note.empty()) alloc.note = "100% Demand satisfied.";
        } else if (alloc.allocatedKwh > 1e-4) {
            alloc.status = FulfillmentStatus::PartiallyMet;
            if (alloc.note.empty()) alloc.note = "Partially satisfied based on priority ranking.";
        } else {
            alloc.status = FulfillmentStatus::Deferred;
            if (alloc.note.empty()) alloc.note = "Deferred: Lower priority under supply constraint.";
        }
    }

    return resultMap;
}

} // namespace smartcampus
