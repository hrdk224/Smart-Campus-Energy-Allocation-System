#include "smartcampus/sources/GridSource.hpp"
#include "smartcampus/common/Exceptions.hpp"
#include "smartcampus/utils/GenericUtils.hpp"
#include <algorithm>

namespace smartcampus {

GridSource::GridSource(std::string id,
                       std::string name,
                       double maxPowerKw,
                       double availabilityFactor,
                       double slotDurationHours)
    : EnergySource(std::move(id), std::move(name), maxPowerKw * slotDurationHours),
      m_maxPowerKw(maxPowerKw),
      m_availabilityFactor(utils::clampValue(availabilityFactor, 0.0, 1.0)),
      m_slotDurationHours(slotDurationHours > 0.0 ? slotDurationHours : 2.0),
      m_cumulativeEnergyDrawnKwh(0.0),
      m_drawnInSlotKwh(0.0) {
    if (maxPowerKw < 0.0) {
        throw NegativeEnergyException("Grid Max Power", maxPowerKw);
    }
}

void GridSource::setAvailabilityFactor(double factor) {
    m_availabilityFactor = utils::clampValue(factor, 0.0, 1.0);
}

double GridSource::getAvailableEnergy() const {
    double totalSlotCapacity = m_maxPowerKw * m_availabilityFactor * m_slotDurationHours;
    return std::max(0.0, totalSlotCapacity - m_drawnInSlotKwh);
}

double GridSource::supply(double requestedKwh) {
    if (requestedKwh < 0.0) {
        throw NegativeEnergyException("Grid Supply Request", requestedKwh);
    }
    double available = getAvailableEnergy();
    double delivered = std::min(requestedKwh, available);
    m_drawnInSlotKwh += delivered;
    m_cumulativeEnergyDrawnKwh += delivered;
    return delivered;
}

std::string GridSource::getSourceName() const {
    return "Municipal Power Grid Substation";
}

void GridSource::resetForTimeSlot() {
    m_drawnInSlotKwh = 0.0;
}

} // namespace smartcampus
