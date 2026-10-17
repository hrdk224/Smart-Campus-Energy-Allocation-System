#include "smartcampus/models/Campus.hpp"
#include <numeric>
#include <iomanip>

namespace smartcampus {

Campus::Campus(std::string campusName)
    : m_campusName(std::move(campusName)) {}

void Campus::addBuilding(std::unique_ptr<Building> building) {
    if (building) {
        m_buildings.push_back(std::move(building));
    }
}

std::vector<Building*> Campus::getBuildingPointers() const {
    std::vector<Building*> ptrs;
    ptrs.reserve(m_buildings.size());
    for (const auto& b : m_buildings) {
        ptrs.push_back(b.get());
    }
    return ptrs;
}

Building* Campus::findBuildingById(const std::string& id) const {
    for (const auto& b : m_buildings) {
        if (b->getId() == id) {
            return b.get();
        }
    }
    return nullptr;
}

double Campus::getTotalEssentialDemand() const {
    double total = 0.0;
    for (const auto& b : m_buildings) {
        total += b->getEssentialDemand();
    }
    return total;
}

double Campus::getTotalDiscretionaryDemand() const {
    double total = 0.0;
    for (const auto& b : m_buildings) {
        total += b->getDiscretionaryDemand();
    }
    return total;
}

double Campus::getTotalDemand() const {
    return getTotalEssentialDemand() + getTotalDiscretionaryDemand();
}

double Campus::getTotalAllocatedEnergy() const {
    double total = 0.0;
    for (const auto& b : m_buildings) {
        total += b->getAllocatedEnergy();
    }
    return total;
}

double Campus::getTotalUnmetDemand() const {
    double total = 0.0;
    for (const auto& b : m_buildings) {
        total += b->getUnmetDemand();
    }
    return total;
}

void Campus::resetAllocations() {
    for (auto& b : m_buildings) {
        b->resetAllocation();
    }
}

std::ostream& operator<<(std::ostream& os, const Campus& campus) {
    os << "=== Campus: " << campus.m_campusName << " (" << campus.m_buildings.size()
       << " buildings registered) ===\n";
    os << "Total Demand: " << campus.getTotalDemand() << " kWh (Essential: "
       << campus.getTotalEssentialDemand() << " kWh, Discretionary: "
       << campus.getTotalDiscretionaryDemand() << " kWh)\n";
    for (const auto& b : campus.m_buildings) {
        os << "  * " << *b << "\n";
    }
    return os;
}

} // namespace smartcampus
