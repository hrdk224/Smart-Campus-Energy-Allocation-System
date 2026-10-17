#include "smartcampus/sources/EnergySource.hpp"
#include "smartcampus/common/Exceptions.hpp"

namespace smartcampus {

EnergySource::EnergySource(std::string sourceId, std::string name, double capacityKwh)
    : m_sourceId(std::move(sourceId)),
      m_name(std::move(name)),
      m_capacityKwh(capacityKwh) {
    if (capacityKwh < 0.0) {
        throw NegativeEnergyException("EnergySource Capacity", capacityKwh);
    }
}

std::ostream& operator<<(std::ostream& os, const EnergySource& source) {
    os << "[" << source.m_sourceId << "] " << source.m_name << " ("
       << source.getSourceName() << ") - Avail: " << source.getAvailableEnergy()
       << " kWh / Cap: " << source.m_capacityKwh << " kWh";
    return os;
}

} // namespace smartcampus
