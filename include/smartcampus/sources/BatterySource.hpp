#ifndef SMARTCAMPUS_BATTERY_SOURCE_HPP
#define SMARTCAMPUS_BATTERY_SOURCE_HPP

#include "smartcampus/sources/EnergySource.hpp"

namespace smartcampus {

/**
 * @brief Campus Battery Energy Storage System (BESS).
 * Demonstrates:
 * - Invariant maintenance (SoC bounds, charge/discharge rates)
 * - State management across time slots
 * - Single Inheritance from EnergySource
 */
class BatterySource : public EnergySource {
private:
    double m_currentSoCKwh;          // Current usable stored energy (kWh)
    double m_maxChargeRateKw;        // Max power accepted during charging (kW)
    double m_maxDischargeRateKw;     // Max power delivered during discharging (kW)
    double m_roundTripEfficiency;    // Efficiency factor (e.g. 0.95 = 95%)
    double m_minReserveRatio;        // Minimum SoC reserve ratio for battery health (e.g. 0.10)
    double m_slotDurationHours;      // Time slot duration in hours

    double m_dischargedInSlotKwh;
    double m_chargedInSlotKwh;

    static constexpr double DEFAULT_EFFICIENCY = 0.95;
    static constexpr double DEFAULT_MIN_RESERVE = 0.10;

public:
    BatterySource(std::string id,
                  std::string name,
                  double capacityKwh,
                  double initialSoCKwh,
                  double maxChargeRateKw = 50.0,
                  double maxDischargeRateKw = 50.0,
                  double efficiency = DEFAULT_EFFICIENCY,
                  double minReserveRatio = DEFAULT_MIN_RESERVE,
                  double slotDurationHours = 2.0);

    ~BatterySource() override = default;

    double getCurrentSoC() const { return m_currentSoCKwh; }
    double getSoCPercentage() const;
    double getMinReserveKwh() const { return m_capacityKwh * m_minReserveRatio; }
    double getDischargedInSlot() const { return m_dischargedInSlotKwh; }
    double getChargedInSlot() const { return m_chargedInSlotKwh; }

    void setSlotDurationHours(double hours) { m_slotDurationHours = hours; }

    // Charge battery with surplus energy; returns actual kWh stored
    double charge(double surplusEnergyKwh);

    // EnergySource overrides
    double getAvailableEnergy() const override;
    double supply(double requestedKwh) override;
    std::string getSourceName() const override;
    void resetForTimeSlot() override;
};

} // namespace smartcampus

#endif // SMARTCAMPUS_BATTERY_SOURCE_HPP
