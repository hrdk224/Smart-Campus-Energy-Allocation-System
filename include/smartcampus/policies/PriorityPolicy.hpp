#ifndef SMARTCAMPUS_PRIORITY_POLICY_HPP
#define SMARTCAMPUS_PRIORITY_POLICY_HPP

#include "smartcampus/policies/AllocationPolicy.hpp"

namespace smartcampus {

/**
 * @brief Priority-based allocation policy.
 * - Serves higher priority buildings first (Critical -> High -> Medium -> Low).
 * - Deterministic tie-breaking by building ID in ascending alphabetical order.
 * - Stage 1: Fulfills essential baseline demands by priority.
 * - Stage 2: Distributes remaining supply to discretionary loads by priority.
 */
class PriorityPolicy : public AllocationPolicy {
public:
    PriorityPolicy() = default;
    ~PriorityPolicy() override = default;

    std::string getPolicyName() const override;
    std::string getDescription() const override;

    std::map<std::string, EnergyAllocation> allocate(
        const std::vector<Building*>& buildings,
        double availableSupplyKwh) override;
};

} // namespace smartcampus

#endif // SMARTCAMPUS_PRIORITY_POLICY_HPP
