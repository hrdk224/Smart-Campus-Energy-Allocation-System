#ifndef SMARTCAMPUS_SOLAR_SOURCE_HPP
#define SMARTCAMPUS_SOLAR_SOURCE_HPP

#include "smartcampus/sources/EnergySource.hpp"

namespace smartcampus {

/**
 * @brief Solar photovoltaic array with intermittent generation based on weather & time.
 * Demonstrates Single Inheritance from EnergySource.
 */
class SolarSource : public EnergySource {
private:
    double m_ratedPeakKw;
    double m_irradianceFactor; // 0.0 (night/overcast) to 1.0 (peak noon sun)
    double m_slotDurationHours;
    double m_deliveredKwhInSlot;

public:
    SolarSource(std::string id,
                std::string name,
                double ratedPeakKw,
                double irradianceFactor = 1.0,
                double slotDurationHours = 2.0);

    ~SolarSource() override = default;

    void setIrradianceFactor(double factor);
    double getIrradianceFactor() const { return m_irradianceFactor; }

    void setSlotDurationHours(double hours);
    double getSlotDurationHours() const { return m_slotDurationHours; }

    // EnergySource overrides
    double getAvailableEnergy() const override;
    double supply(double requestedKwh) override;
    std::string getSourceName() const override;
    void resetForTimeSlot() override;
};

} // namespace smartcampus

#endif // SMARTCAMPUS_SOLAR_SOURCE_HPP
