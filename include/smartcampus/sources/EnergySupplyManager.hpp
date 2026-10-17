#ifndef SMARTCAMPUS_ENERGY_SUPPLY_MANAGER_HPP
#define SMARTCAMPUS_ENERGY_SUPPLY_MANAGER_HPP

#include "smartcampus/sources/SolarSource.hpp"
#include "smartcampus/sources/BatterySource.hpp"
#include "smartcampus/sources/GridSource.hpp"
#include "smartcampus/common/Types.hpp"
#include <memory>

namespace smartcampus {

/**
 * @brief Coordinates generation, storage, and grid backup across time slots.
 * Enforces green dispatch priority:
 *   1. Dispatch Solar (zero marginal cost, 100% renewable)
 *   2. If Solar has surplus over demand, charge Battery
 *   3. If Solar is insufficient, discharge Battery (stored green energy)
 *   4. If still insufficient, draw from Grid backup
 */
class EnergySupplyManager {
private:
    std::unique_ptr<SolarSource> m_solar;
    std::unique_ptr<BatterySource> m_battery;
    std::unique_ptr<GridSource> m_grid;

public:
    EnergySupplyManager(std::unique_ptr<SolarSource> solar,
                        std::unique_ptr<BatterySource> battery,
                        std::unique_ptr<GridSource> grid);

    ~EnergySupplyManager() = default;

    // Disallow copy, allow move
    EnergySupplyManager(const EnergySupplyManager&) = delete;
    EnergySupplyManager& operator=(const EnergySupplyManager&) = delete;
    EnergySupplyManager(EnergySupplyManager&&) noexcept = default;
    EnergySupplyManager& operator=(EnergySupplyManager&&) noexcept = default;

    SolarSource& getSolar() { return *m_solar; }
    const SolarSource& getSolar() const { return *m_solar; }

    BatterySource& getBattery() { return *m_battery; }
    const BatterySource& getBattery() const { return *m_battery; }

    GridSource& getGrid() { return *m_grid; }
    const GridSource& getGrid() const { return *m_grid; }

    // Prepare all sources for a new time slot
    void resetForTimeSlot();

    // Compute total available generation & storage for the slot
    double getTotalAvailableSupply() const;

    /**
     * @brief Execute physical dispatch to meet requested demand.
     * @param totalDemandRequested Campus aggregate demand to fulfill
     * @return EnergySupplySummary detailing exact kWh drawn from each source and battery state
     */
    EnergySupplySummary dispatchEnergy(double totalDemandRequested);
};

} // namespace smartcampus

#endif // SMARTCAMPUS_ENERGY_SUPPLY_MANAGER_HPP
