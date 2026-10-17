#ifndef SMARTCAMPUS_RESIDENTIAL_BUILDING_HPP
#define SMARTCAMPUS_RESIDENTIAL_BUILDING_HPP

#include "smartcampus/models/Building.hpp"

namespace smartcampus {

/**
 * @brief Base class for residential facilities on campus (hostels, faculty apartments).
 * Demonstrates Single Inheritance from Building.
 */
class ResidentialBuilding : public Building {
private:
    int m_residentCapacity;

public:
    ResidentialBuilding(std::string id,
                        std::string name,
                        PriorityLevel priority,
                        double essentialDemand,
                        double discretionaryDemand,
                        int residentCapacity);

    ~ResidentialBuilding() override = default;

    int getResidentCapacity() const { return m_residentCapacity; }

    std::string getTypeName() const override;
    void allocate(double energyKwh) override;
};

} // namespace smartcampus

#endif // SMARTCAMPUS_RESIDENTIAL_BUILDING_HPP
