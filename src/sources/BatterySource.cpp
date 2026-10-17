#include "smartcampus/sources/BatterySource.hpp"
#include "smartcampus/common/Exceptions.hpp"
#include "smartcampus/utils/GenericUtils.hpp"
#include <algorithm>

namespace smartcampus {

BatterySource::BatterySource(std::string id,
                             std::string name,
                             double capacityKwh,
                             double initialSoCKwh,
                             double maxChargeRateKw,
                             double maxDischargeRateKw,
                             double efficiency,
                             double minReserveRatio,
                             double slotDurationHours)
    : EnergySource(std::move(id), std::move(name), capacityKwh),
      m_currentSoCKwh(initialSoCKwh),
      m_maxChargeRateKw(maxChargeRateKw),
      m_maxDischargeRateKw(maxDischargeRateKw),
      m_roundTripEfficiency(utils::clampValue(efficiency, 0.50, 1.0)),
      m_minReserveRatio(utils::clampValue(minReserveRatio, 0.0, 0.50)),
      m_slotDurationHours(slotDurationHours > 0.0 ? slotDurationHours : 2.0),
      m_dischargedInSlotKwh(0.0),
      m_chargedInSlotKwh(0.0) {
    if (capacityKwh <= 0.0) {
        throw BatteryOverdrawException("Battery capacity must be positive: " + std::to_string(capacityKwh));
    }
    if (initialSoCKwh < 0.0 || initialSoCKwh > capacityKwh) {
        throw BatteryOverdrawException("Initial SoC (" + std::to_string(initialSoCKwh) +
                                       " kWh) must be between 0 and capacity (" +
                                       std::to_string(capacityKwh) + " kWh)");
    }
}

double BatterySource::getSoCPercentage() const {
    if (m_capacityKwh <= 0.0) return 0.0;
    return (m_currentSoCKwh / m_capacityKwh) * 100.0;
}

double BatterySource::getAvailableEnergy() const {
    double minReserve = getMinReserveKwh();
    double usableSoC = std::max(0.0, m_currentSoCKwh - minReserve);
    double maxDischargeThisSlot = m_maxDischargeRateKw * m_slotDurationHours;
    double remainingSlotLimit = std::max(0.0, maxDischargeThisSlot - m_dischargedInSlotKwh);
    return std::min(usableSoC, remainingSlotLimit);
}

double BatterySource::supply(double requestedKwh) {
    if (requestedKwh < 0.0) {
        throw NegativeEnergyException("Battery Supply Request", requestedKwh);
    }
    double available = getAvailableEnergy();
    double delivered = std::min(requestedKwh, available);

    m_currentSoCKwh -= delivered;
    m_dischargedInSlotKwh += delivered;
    return delivered;
}

double BatterySource::charge(double surplusEnergyKwh) {
    if (surplusEnergyKwh < 0.0) {
        throw NegativeEnergyException("Battery Charge Energy", surplusEnergyKwh);
    }
    double headroom = std::max(0.0, m_capacityKwh - m_currentSoCKwh);
    double maxChargeThisSlot = m_maxChargeRateKw * m_slotDurationHours;
    double maxCanAbsorb = std::min(headroom, maxChargeThisSlot);

    // Apply round-trip / charging efficiency
    double effectiveCharge = std::min(surplusEnergyKwh * m_roundTripEfficiency, maxCanAbsorb);
    m_currentSoCKwh += effectiveCharge;
    m_chargedInSlotKwh += effectiveCharge;

    // Return the actual raw surplus energy absorbed from grid/solar
    return (m_roundTripEfficiency > 0.0) ? (effectiveCharge / m_roundTripEfficiency) : 0.0;
}

std::string BatterySource::getSourceName() const {
    return "Lithium-Ion Battery Storage System";
}

void BatterySource::resetForTimeSlot() {
    m_dischargedInSlotKwh = 0.0;
    m_chargedInSlotKwh = 0.0;
}

} // namespace smartcampus
