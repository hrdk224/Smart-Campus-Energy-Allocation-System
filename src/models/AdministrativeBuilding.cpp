#include "smartcampus/models/AdministrativeBuilding.hpp"
#include <algorithm>

namespace smartcampus {

AdministrativeBuilding::AdministrativeBuilding(std::string id,
                                               std::string name,
                                               PriorityLevel priority,
                                               double essentialDemand,
                                               double discretionaryDemand,
                                               std::string department,
                                               bool hasDataServerRoom)
    : Building(std::move(id),
               std::move(name),
               BuildingCategory::Administrative,
               priority,
               essentialDemand,
               discretionaryDemand),
      m_department(std::move(department)),
      m_hasDataServerRoom(hasDataServerRoom) {}

std::string AdministrativeBuilding::getTypeName() const {
    return "Administrative Building";
}

void AdministrativeBuilding::allocate(double energyKwh) {
    double capped = std::min(energyKwh, getTotalDemand());
    setAllocatedEnergy(capped);
}

} // namespace smartcampus
