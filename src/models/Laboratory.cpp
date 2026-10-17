#include "smartcampus/models/Laboratory.hpp"
#include <algorithm>

namespace smartcampus {

Laboratory::Laboratory(std::string id,
                       std::string name,
                       PriorityLevel priority,
                       double essentialDemand,
                       double discretionaryDemand,
                       int classroomCount,
                       bool hasSmartBoards,
                       int hazardousFumeHoods,
                       bool backupGeneratorRequired)
    : AcademicBuilding(std::move(id),
                       std::move(name),
                       priority,
                       essentialDemand,
                       discretionaryDemand,
                       classroomCount,
                       hasSmartBoards),
      m_hazardousFumeHoods(hazardousFumeHoods),
      m_backupGeneratorRequired(backupGeneratorRequired),
      m_safetyCriticalBaselineKwh(essentialDemand * 0.8) {}

std::string Laboratory::getTypeName() const {
    return "Specialized Research Laboratory";
}

void Laboratory::allocate(double energyKwh) {
    // Laboratory validates that allocation is capped at requested demand
    double capped = std::min(energyKwh, getTotalDemand());
    setAllocatedEnergy(capped);
}

} // namespace smartcampus
