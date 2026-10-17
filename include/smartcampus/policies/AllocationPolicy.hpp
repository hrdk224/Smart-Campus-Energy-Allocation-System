#ifndef SMARTCAMPUS_ALLOCATION_POLICY_HPP
#define SMARTCAMPUS_ALLOCATION_POLICY_HPP

#include "smartcampus/models/Building.hpp"
#include "smartcampus/common/Types.hpp"
#include <vector>
#include <map>
#include <string>

namespace smartcampus {

/**
 * @brief Strategy interface for energy allocation algorithms.
 *
 * KEY OOP CONCEPTS DEMONSTRATED:
 * 1. Strategy Design Pattern: Encapsulates swappable allocation algorithms
 * 2. Pure Virtual Interface: Guarantees polymorphic allocate() contract
 * 3. Open/Closed Principle: Add new policies without modifying AllocationEngine
 */
// =============================================================================
// OOP CONCEPT: STRATEGY PATTERN & PURE ABSTRACT INTERFACE
// =============================================================================
class AllocationPolicy {
public:
    virtual ~AllocationPolicy() = default;

    virtual std::string getPolicyName() const = 0;
    virtual std::string getDescription() const = 0;

    /**
     * @brief Compute energy allocation for each building given available supply.
     * Runtime Polymorphism: Called via base pointer in AllocationEngine.
     */
    virtual std::map<std::string, EnergyAllocation> allocate(
        const std::vector<Building*>& buildings,
        double availableSupplyKwh) = 0;
};

} // namespace smartcampus

#endif // SMARTCAMPUS_ALLOCATION_POLICY_HPP
