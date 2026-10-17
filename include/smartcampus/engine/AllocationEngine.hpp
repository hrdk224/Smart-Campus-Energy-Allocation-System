#ifndef SMARTCAMPUS_ALLOCATION_ENGINE_HPP
#define SMARTCAMPUS_ALLOCATION_ENGINE_HPP

#include "smartcampus/models/Campus.hpp"
#include "smartcampus/sources/EnergySupplyManager.hpp"
#include "smartcampus/policies/AllocationPolicy.hpp"
#include "smartcampus/engine/AllocationRecord.hpp"
#include <vector>
#include <memory>

namespace smartcampus {

/**
 * @brief Central simulation engine executing energy allocation algorithms.
 *
 * KEY OOP CONCEPTS DEMONSTRATED:
 * 1. Aggregation / Association: Works through abstract AllocationPolicy interface
 * 2. Invariant Validation: Defensive programming and custom exception throws
 * 3. Separation of Concerns: Coordinates Campus and Sources without owning them
 */
class AllocationEngine {
    // =========================================================================
    // OOP CONCEPT: AGGREGATION & ASSOCIATION ("USES-A")
    // AllocationEngine holds a shared pointer to AllocationPolicy.
    // The policy can be changed dynamically at runtime via setPolicy().
    // =========================================================================
private:
    Campus& m_campus;
    EnergySupplyManager& m_supplyManager;
    std::shared_ptr<AllocationPolicy> m_currentPolicy;
    std::vector<AllocationRecord> m_simulationHistory;

    // Internal invariant verification (allocated <= supply, allocated >= 0)
    void validateInvariants(double totalAllocated,
                            double availableSupply,
                            const std::map<std::string, EnergyAllocation>& allocations) const;

public:
    AllocationEngine(Campus& campus,
                     EnergySupplyManager& supplyManager,
                     std::shared_ptr<AllocationPolicy> initialPolicy);

    ~AllocationEngine() = default;

    AllocationEngine(const AllocationEngine&) = delete;
    AllocationEngine& operator=(const AllocationEngine&) = delete;

    // =========================================================================
    // OOP CONCEPT: DYNAMIC RUNTIME STRATEGY SWAPPING
    // =========================================================================
    void setPolicy(std::shared_ptr<AllocationPolicy> policy);
    const AllocationPolicy& getPolicy() const;
    std::shared_ptr<AllocationPolicy> getPolicyPtr() const { return m_currentPolicy; }

    /**
     * @brief Execute simulation for a discrete time slot.
     */
    AllocationRecord simulateTimeSlot(const TimeSlot& slot);

    const std::vector<AllocationRecord>& getHistory() const { return m_simulationHistory; }
    void clearHistory() { m_simulationHistory.clear(); }
};

} // namespace smartcampus

#endif // SMARTCAMPUS_ALLOCATION_ENGINE_HPP
