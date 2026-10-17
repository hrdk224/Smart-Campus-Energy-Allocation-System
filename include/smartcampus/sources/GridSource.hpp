#ifndef SMARTCAMPUS_GRID_SOURCE_HPP
#define SMARTCAMPUS_GRID_SOURCE_HPP

#include "smartcampus/sources/EnergySource.hpp"

namespace smartcampus {

/**
 * @brief Municipal power grid connection serving as campus backup / baseline.
 * Demonstrates Single Inheritance from EnergySource.
 */
class GridSource : public EnergySource {
private:
    double m_maxPowerKw;             // Substation connection capacity (kW)
    double m_availabilityFactor;     // 1.0 = normal, 0.5 = brownout, 0.0 = blackout
    double m_slotDurationHours;
    double m_cumulativeEnergyDrawnKwh;
    double m_drawnInSlotKwh;

public:
    GridSource(std::string id,
               std::string name,
               double maxPowerKw = 250.0,
               double availabilityFactor = 1.0,
               double slotDurationHours = 2.0);

    ~GridSource() override = default;

    void setAvailabilityFactor(double factor);
    double getAvailabilityFactor() const { return m_availabilityFactor; }

    void setSlotDurationHours(double hours) { m_slotDurationHours = hours; }
    double getCumulativeEnergyDrawn() const { return m_cumulativeEnergyDrawnKwh; }
    double getDrawnInSlot() const { return m_drawnInSlotKwh; }

    // EnergySource overrides
    double getAvailableEnergy() const override;
    double supply(double requestedKwh) override;
    std::string getSourceName() const override;
    void resetForTimeSlot() override;
};

} // namespace smartcampus

#endif // SMARTCAMPUS_GRID_SOURCE_HPP
