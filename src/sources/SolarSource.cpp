#include "smartcampus/sources/SolarSource.hpp"
#include "smartcampus/common/Exceptions.hpp"
#include "smartcampus/utils/GenericUtils.hpp"
#include <algorithm>

namespace smartcampus {

SolarSource::SolarSource(std::string id,
                         std::string name,
                         double ratedPeakKw,
                         double irradianceFactor,
                         double slotDurationHours)
    : EnergySource(std::move(id), std::move(name), ratedPeakKw * slotDurationHours),
      m_ratedPeakKw(ratedPeakKw),
      m_irradianceFactor(utils::clampValue(irradianceFactor, 0.0, 1.0)),
      m_slotDurationHours(slotDurationHours > 0.0 ? slotDurationHours : 2.0),
      m_deliveredKwhInSlot(0.0) {
    if (ratedPeakKw < 0.0) {
        throw NegativeEnergyException("Solar Rated Peak", ratedPeakKw);
    }
}

void SolarSource::setIrradianceFactor(double factor) {
    m_irradianceFactor = utils::clampValue(factor, 0.0, 1.0);
}

void SolarSource::setSlotDurationHours(double hours) {
    if (hours <= 0.0) {
        throw std::invalid_argument("Slot duration must be positive");
    }
    m_slotDurationHours = hours;
    m_capacityKwh = m_ratedPeakKw * m_slotDurationHours;
}

double SolarSource::getAvailableEnergy() const {
    double totalGen = m_ratedPeakKw * m_irradianceFactor * m_slotDurationHours;
    return std::max(0.0, totalGen - m_deliveredKwhInSlot);
}

double SolarSource::supply(double requestedKwh) {
    if (requestedKwh < 0.0) {
        throw NegativeEnergyException("Requested Solar", requestedKwh);
    }
    double available = getAvailableEnergy();
    double delivered = std::min(requestedKwh, available);
    m_deliveredKwhInSlot += delivered;
    return delivered;
}

std::string SolarSource::getSourceName() const {
    return "Photovoltaic Solar Array";
}

void SolarSource::resetForTimeSlot() {
    m_deliveredKwhInSlot = 0.0;
}

} // namespace smartcampus
