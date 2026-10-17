#ifndef SMARTCAMPUS_ACADEMIC_BUILDING_HPP
#define SMARTCAMPUS_ACADEMIC_BUILDING_HPP

#include "smartcampus/models/Building.hpp"

namespace smartcampus {

/**
 * @brief Represents standard academic lecture complexes, seminar halls, and faculties.
 * Demonstrates Single Inheritance from Building.
 */
class AcademicBuilding : public Building {
private:
    int m_classroomCount;
    bool m_hasSmartBoards;

public:
    AcademicBuilding(std::string id,
                     std::string name,
                     PriorityLevel priority,
                     double essentialDemand,
                     double discretionaryDemand,
                     int classroomCount,
                     bool hasSmartBoards);

    ~AcademicBuilding() override = default;

    int getClassroomCount() const { return m_classroomCount; }
    bool hasSmartBoards() const { return m_hasSmartBoards; }

    // Overridden methods
    std::string getTypeName() const override;
    void allocate(double energyKwh) override;
};

} // namespace smartcampus

#endif // SMARTCAMPUS_ACADEMIC_BUILDING_HPP
