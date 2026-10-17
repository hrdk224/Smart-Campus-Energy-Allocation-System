#include "smartcampus/models/AcademicBuilding.hpp"
#include <algorithm>

namespace smartcampus {

AcademicBuilding::AcademicBuilding(std::string id,
                                   std::string name,
                                   PriorityLevel priority,
                                   double essentialDemand,
                                   double discretionaryDemand,
                                   int classroomCount,
                                   bool hasSmartBoards)
    : Building(std::move(id),
               std::move(name),
               BuildingCategory::Academic,
               priority,
               essentialDemand,
               discretionaryDemand),
      m_classroomCount(classroomCount),
      m_hasSmartBoards(hasSmartBoards) {}

std::string AcademicBuilding::getTypeName() const {
    return "Academic Building";
}

void AcademicBuilding::allocate(double energyKwh) {
    // Academic building accepts energy up to total demand
    double capped = std::min(energyKwh, getTotalDemand());
    setAllocatedEnergy(capped);
}

} // namespace smartcampus
