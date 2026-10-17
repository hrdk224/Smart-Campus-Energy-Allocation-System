#include "smartcampus/sources/EnergySupplyManager.hpp"
#include <algorithm>

namespace smartcampus {

EnergySupplyManager::EnergySupplyManager(std::unique_ptr<SolarSource> solar,
                                         std::unique_ptr<BatterySource> battery,
                                         std::unique_ptr<GridSource> grid)
    : m_solar(std::move(solar)),
      m_battery(std::move(battery)),
      m_grid(std::move(grid)) {}

void EnergySupplyManager::resetForTimeSlot() {
    if (m_solar) m_solar->resetForTimeSlot();
    if (m_battery) m_battery->resetForTimeSlot();
    if (m_grid) m_grid->resetForTimeSlot();
}

double EnergySupplyManager::getTotalAvailableSupply() const {
    double total = 0.0;
    if (m_solar) total += m_solar->getAvailableEnergy();
    if (m_battery) total += m_battery->getAvailableEnergy();
    if (m_grid) total += m_grid->getAvailableEnergy();
    return total;
}

EnergySupplySummary EnergySupplyManager::dispatchEnergy(double totalDemandRequested) {
    EnergySupplySummary summary;

    summary.batteryStartSoCKwh = m_battery ? m_battery->getCurrentSoC() : 0.0;
    summary.solarAvailableKwh = m_solar ? m_solar->getAvailableEnergy() : 0.0;
    summary.batteryAvailableKwh = m_battery ? m_battery->getAvailableEnergy() : 0.0;
    summary.gridAvailableKwh = m_grid ? m_grid->getAvailableEnergy() : 0.0;

    double needed = totalDemandRequested;

    // Step 1: Solar dispatch (cleanest source)
    if (m_solar && summary.solarAvailableKwh > 0.0) {
        if (summary.solarAvailableKwh >= needed) {
            // Solar alone can cover all demand!
            summary.solarUsedKwh = m_solar->supply(needed);
            double solarSurplus = summary.solarAvailableKwh - needed;
            needed = 0.0;

            // Route surplus solar into battery storage
            if (m_battery && solarSurplus > 0.0) {
                summary.batteryChargedKwh = m_battery->charge(solarSurplus);
            }
        } else {
            // Solar provides part of the demand
            summary.solarUsedKwh = m_solar->supply(summary.solarAvailableKwh);
            needed -= summary.solarUsedKwh;
        }
    }

    // Step 2: Battery discharge (stored clean energy)
    if (needed > 0.0 && m_battery && summary.batteryAvailableKwh > 0.0) {
        summary.batteryDischargedKwh = m_battery->supply(needed);
        needed -= summary.batteryDischargedKwh;
    }

    // Step 3: Grid backup (reliable baseline / fallback)
    if (needed > 0.0 && m_grid && summary.gridAvailableKwh > 0.0) {
        summary.gridUsedKwh = m_grid->supply(needed);
        needed -= summary.gridUsedKwh;
    }

    summary.batteryEndSoCKwh = m_battery ? m_battery->getCurrentSoC() : 0.0;

    return summary;
}

} // namespace smartcampus
