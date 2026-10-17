#ifndef SMARTCAMPUS_LABORATORY_HPP
#define SMARTCAMPUS_LABORATORY_HPP

#include "smartcampus/models/AcademicBuilding.hpp"

namespace smartcampus {

/**
 * @brief Represents specialized research laboratories (AI servers, chemistry labs).
 *
 * KEY OOP CONCEPTS DEMONSTRATED:
 * 1. Multilevel Inheritance: Building -> AcademicBuilding -> Laboratory
 * 2. Specialized Polymorphic Behavior: Enforces safety-critical baselines
 */
// =============================================================================
// OOP CONCEPT: MULTILEVEL INHERITANCE
// Building (Grandparent) -> AcademicBuilding (Parent) -> Laboratory (Child)
// Laboratory inherits classrooms from AcademicBuilding AND ID/demands from Building.
// =============================================================================
class Laboratory : public AcademicBuilding {
private:
    int m_hazardousFumeHoods;
    bool m_backupGeneratorRequired;
    double m_safetyCriticalBaselineKwh;

public:
    Laboratory(std::string id,
               std::string name,
               PriorityLevel priority,
               double essentialDemand,
               double discretionaryDemand,
               int classroomCount,
               bool hasSmartBoards,
               int hazardousFumeHoods,
               bool backupGeneratorRequired);

    ~Laboratory() override = default;

    int getFumeHoodCount() const { return m_hazardousFumeHoods; }
    bool isBackupGeneratorRequired() const { return m_backupGeneratorRequired; }
    double getSafetyCriticalBaseline() const { return m_safetyCriticalBaselineKwh; }

    // =========================================================================
    // OOP CONCEPT: METHOD OVERRIDING (RUNTIME POLYMORPHISM)
    // =========================================================================
    std::string getTypeName() const override;
    void allocate(double energyKwh) override;
};

} // namespace smartcampus

#endif // SMARTCAMPUS_LABORATORY_HPP
