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
 * Demonstrates:
 * - Strategy Design Pattern
 * - Pure virtual interface
 * - Decoupling algorithm implementation from the AllocationEngine
 */
class AllocationPolicy {
public:
    virtual ~AllocationPolicy() = default;

    virtual std::string getPolicyName() const = 0;
    virtual std::string getDescription() const = 0;

    /**
     * @brief Compute energy allocation for each building given available supply.
     * @param buildings Non-owning pointers to registered campus buildings
     * @param availableSupplyKwh Total electrical energy available for the slot
     * @return Map of Building ID to EnergyAllocation record
     */
    virtual std::map<std::string, EnergyAllocation> allocate(
        const std::vector<Building*>& buildings,
        double availableSupplyKwh) = 0;
};

} // namespace smartcampus

#endif // SMARTCAMPUS_ALLOCATION_POLICY_HPP
