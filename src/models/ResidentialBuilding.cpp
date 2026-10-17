#include "smartcampus/models/ResidentialBuilding.hpp"
#include <algorithm>

namespace smartcampus {

ResidentialBuilding::ResidentialBuilding(std::string id,
                                         std::string name,
                                         PriorityLevel priority,
                                         double essentialDemand,
                                         double discretionaryDemand,
                                         int residentCapacity)
    : Building(std::move(id),
               std::move(name),
               BuildingCategory::Residential,
               priority,
               essentialDemand,
               discretionaryDemand),
      m_residentCapacity(residentCapacity) {}

std::string ResidentialBuilding::getTypeName() const {
    return "Residential Building";
}

void ResidentialBuilding::allocate(double energyKwh) {
    double capped = std::min(energyKwh, getTotalDemand());
    setAllocatedEnergy(capped);
}

} // namespace smartcampus
