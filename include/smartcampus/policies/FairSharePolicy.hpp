#ifndef SMARTCAMPUS_FAIR_SHARE_POLICY_HPP
#define SMARTCAMPUS_FAIR_SHARE_POLICY_HPP

#include "smartcampus/policies/AllocationPolicy.hpp"

namespace smartcampus {

/**
 * @brief Proportional Fair-Share allocation policy.
 * - Stage 1: Protects essential demands for all buildings.
 * - Stage 2: Distributes remaining supply proportionally based on each building's
 *   requested discretionary demand ratio, preventing starvation of lower-priority buildings.
 * - In severe shortage: Scales down essential allocations proportionally across all buildings.
 */
class FairSharePolicy : public AllocationPolicy {
public:
    FairSharePolicy() = default;
    ~FairSharePolicy() override = default;

    std::string getPolicyName() const override;
    std::string getDescription() const override;

    std::map<std::string, EnergyAllocation> allocate(
        const std::vector<Building*>& buildings,
        double availableSupplyKwh) override;
};

} // namespace smartcampus

#endif // SMARTCAMPUS_FAIR_SHARE_POLICY_HPP
