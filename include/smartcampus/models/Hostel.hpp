#ifndef SMARTCAMPUS_HOSTEL_HPP
#define SMARTCAMPUS_HOSTEL_HPP

#include "smartcampus/models/ResidentialBuilding.hpp"

namespace smartcampus {

/**
 * @brief Represents student hostels with specific load characteristics (e.g. evening dining halls, laundry).
 * Demonstrates Multilevel Inheritance: Building -> ResidentialBuilding -> Hostel.
 */
class Hostel : public ResidentialBuilding {
private:
    int m_numberOfRooms;
    bool m_hasDiningMess;
    double m_messKitchenLoadKwh;

public:
    Hostel(std::string id,
           std::string name,
           PriorityLevel priority,
           double essentialDemand,
           double discretionaryDemand,
           int residentCapacity,
           int numberOfRooms,
           bool hasDiningMess);

    ~Hostel() override = default;

    int getRoomCount() const { return m_numberOfRooms; }
    bool hasDiningMess() const { return m_hasDiningMess; }
    double getMessKitchenLoad() const { return m_messKitchenLoadKwh; }

    std::string getTypeName() const override;
    void allocate(double energyKwh) override;
};

} // namespace smartcampus

#endif // SMARTCAMPUS_HOSTEL_HPP
