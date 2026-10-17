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
 * Demonstrates:
 * - Aggregation / Association: interacts with AllocationPolicy through its interface
 * - Invariant verification (conservation of energy, non-negativity)
 * - Encapsulation of simulation workflow
 */
class AllocationEngine {
private:
    Campus& m_campus;
    EnergySupplyManager& m_supplyManager;
    std::shared_ptr<AllocationPolicy> m_currentPolicy;
    std::vector<AllocationRecord> m_simulationHistory;

    // Internal invariant validation helper
    void validateInvariants(double totalAllocated,
                            double availableSupply,
                            const std::map<std::string, EnergyAllocation>& allocations) const;

public:
    AllocationEngine(Campus& campus,
                     EnergySupplyManager& supplyManager,
                     std::shared_ptr<AllocationPolicy> initialPolicy);

    ~AllocationEngine() = default;

    // Disallow copy, allow move
    AllocationEngine(const AllocationEngine&) = delete;
    AllocationEngine& operator=(const AllocationEngine&) = delete;

    void setPolicy(std::shared_ptr<AllocationPolicy> policy);
    const AllocationPolicy& getPolicy() const;
    std::shared_ptr<AllocationPolicy> getPolicyPtr() const { return m_currentPolicy; }

    /**
     * @brief Execute simulation for a discrete time slot.
     * Steps:
     *   1. Prepare sources and campus for the slot
     *   2. Compute total available supply
     *   3. Run policy allocation algorithm
     *   4. Verify all mathematical invariants
     *   5. Physically dispatch energy from sources
     *   6. Apply allocations to individual Building instances
     *   7. Store and return AllocationRecord
     */
    AllocationRecord simulateTimeSlot(const TimeSlot& slot);

    const std::vector<AllocationRecord>& getHistory() const { return m_simulationHistory; }
    void clearHistory() { m_simulationHistory.clear(); }
};

} // namespace smartcampus

#endif // SMARTCAMPUS_ALLOCATION_ENGINE_HPP
