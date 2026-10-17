#ifndef SMARTCAMPUS_CAMPUS_HPP
#define SMARTCAMPUS_CAMPUS_HPP

#include "smartcampus/models/Building.hpp"
#include <vector>
#include <memory>
#include <string>

namespace smartcampus {

/**
 * @brief Represents the physical campus managing all registered buildings.
 * Demonstrates:
 * - Composition: owns Building instances via unique_ptr
 * - Method Overloading: compile-time polymorphism for adding buildings
 * - const correctness
 */
class Campus {
private:
    std::string m_campusName;
    std::vector<std::unique_ptr<Building>> m_buildings;

public:
    explicit Campus(std::string campusName = "Smart Green University");
    ~Campus() = default;

    // Disallow copying to prevent duplicate ownership of unique_ptrs
    Campus(const Campus&) = delete;
    Campus& operator=(const Campus&) = delete;

    // Allow moving
    Campus(Campus&&) noexcept = default;
    Campus& operator=(Campus&&) noexcept = default;

    const std::string& getName() const { return m_campusName; }
    size_t getBuildingCount() const { return m_buildings.size(); }

    // Method Overloading (Compile-Time Polymorphism)
    void addBuilding(std::unique_ptr<Building> building);

    // Observer pointers for non-owning callers like AllocationEngine
    std::vector<Building*> getBuildingPointers() const;
    const std::vector<std::unique_ptr<Building>>& getBuildings() const { return m_buildings; }

    Building* findBuildingById(const std::string& id) const;

    // Aggregated demand computations
    double getTotalEssentialDemand() const;
    double getTotalDiscretionaryDemand() const;
    double getTotalDemand() const;
    double getTotalAllocatedEnergy() const;
    double getTotalUnmetDemand() const;

    // Reset all buildings' allocations for a new time slot
    void resetAllocations();

    // Stream operator overload
    friend std::ostream& operator<<(std::ostream& os, const Campus& campus);
};

} // namespace smartcampus

#endif // SMARTCAMPUS_CAMPUS_HPP
