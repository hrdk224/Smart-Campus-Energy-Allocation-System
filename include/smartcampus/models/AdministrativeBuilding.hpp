#ifndef SMARTCAMPUS_ADMINISTRATIVE_BUILDING_HPP
#define SMARTCAMPUS_ADMINISTRATIVE_BUILDING_HPP

#include "smartcampus/models/Building.hpp"

namespace smartcampus {

/**
 * @brief Represents administrative complexes, libraries, or administrative centers.
 * Demonstrates Single Inheritance from Building.
 */
class AdministrativeBuilding : public Building {
private:
    std::string m_department;
    bool m_hasDataServerRoom;

public:
    AdministrativeBuilding(std::string id,
                           std::string name,
                           PriorityLevel priority,
                           double essentialDemand,
                           double discretionaryDemand,
                           std::string department,
                           bool hasDataServerRoom);

    ~AdministrativeBuilding() override = default;

    const std::string& getDepartment() const { return m_department; }
    bool hasDataServerRoom() const { return m_hasDataServerRoom; }

    std::string getTypeName() const override;
    void allocate(double energyKwh) override;
};

} // namespace smartcampus

#endif // SMARTCAMPUS_ADMINISTRATIVE_BUILDING_HPP
