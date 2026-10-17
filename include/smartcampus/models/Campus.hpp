#ifndef SMARTCAMPUS_CAMPUS_HPP
#define SMARTCAMPUS_CAMPUS_HPP

#include "smartcampus/models/Building.hpp"
#include <vector>
#include <memory>
#include <string>

namespace smartcampus {

/**
 * @brief Represents the physical campus managing all registered buildings.
 *
 * KEY OOP CONCEPTS DEMONSTRATED:
 * 1. Composition: Campus has-a vector of unique_ptr<Building> (exclusive ownership)
 * 2. Smart Pointers: std::unique_ptr for automatic RAII memory management
 * 3. Method Overloading: Compile-time polymorphism
 * 4. Const Correctness: Read-only accessors
 */
class Campus {
    // =========================================================================
    // OOP CONCEPT: COMPOSITION & SMART POINTERS
    // Campus strictly owns the lifecycle of its Building objects via unique_ptr.
    // If Campus is destroyed, all contained buildings are deallocated automatically.
    // =========================================================================
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

    void addBuilding(std::unique_ptr<Building> building);

    // Non-owning observer pointers for allocation algorithms
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
