#ifndef SMARTCAMPUS_LABORATORY_HPP
#define SMARTCAMPUS_LABORATORY_HPP

#include "smartcampus/models/AcademicBuilding.hpp"

namespace smartcampus {

/**
 * @brief Represents specialized research laboratories (e.g., Chemistry, High-Performance Computing, Robotics).
 * Demonstrates Multilevel Inheritance: Building -> AcademicBuilding -> Laboratory.
 * Laboratories enforce non-negotiable critical essential baseline (fume hoods, cooling servers).
 */
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

    // Overridden polymorphic methods
    std::string getTypeName() const override;
    void allocate(double energyKwh) override;
};

} // namespace smartcampus

#endif // SMARTCAMPUS_LABORATORY_HPP
