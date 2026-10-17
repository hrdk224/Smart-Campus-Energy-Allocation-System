#include "smartcampus/models/Hostel.hpp"
#include <algorithm>

namespace smartcampus {

Hostel::Hostel(std::string id,
               std::string name,
               PriorityLevel priority,
               double essentialDemand,
               double discretionaryDemand,
               int residentCapacity,
               int numberOfRooms,
               bool hasDiningMess)
    : ResidentialBuilding(std::move(id),
                          std::move(name),
                          priority,
                          essentialDemand,
                          discretionaryDemand,
                          residentCapacity),
      m_numberOfRooms(numberOfRooms),
      m_hasDiningMess(hasDiningMess),
      m_messKitchenLoadKwh(hasDiningMess ? essentialDemand * 0.4 : 0.0) {}

std::string Hostel::getTypeName() const {
    return "Student Hostel";
}

void Hostel::allocate(double energyKwh) {
    double capped = std::min(energyKwh, getTotalDemand());
    setAllocatedEnergy(capped);
}

} // namespace smartcampus
