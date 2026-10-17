#ifndef SMARTCAMPUS_ACADEMIC_BUILDING_HPP
#define SMARTCAMPUS_ACADEMIC_BUILDING_HPP

#include "smartcampus/models/Building.hpp"

namespace smartcampus {

/**
 * @brief Represents standard academic lecture complexes and faculties.
 *
 * KEY OOP CONCEPTS DEMONSTRATED:
 * 1. Single Inheritance: AcademicBuilding inherits directly from Building
 * 2. Method Overriding: Implements pure virtual getTypeName() and allocate()
 */
// =============================================================================
// OOP CONCEPT: SINGLE INHERITANCE
// AcademicBuilding is-a Building, inheriting ID, priority, and demand state.
// =============================================================================
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

    // =========================================================================
    // OOP CONCEPT: RUNTIME POLYMORPHISM (OVERRIDING VIRTUAL METHODS)
    // =========================================================================
    std::string getTypeName() const override;
    void allocate(double energyKwh) override;
};

} // namespace smartcampus

#endif // SMARTCAMPUS_ACADEMIC_BUILDING_HPP
