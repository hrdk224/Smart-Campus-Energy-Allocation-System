#ifndef SMARTCAMPUS_BUILDING_HPP
#define SMARTCAMPUS_BUILDING_HPP

#include "smartcampus/common/Types.hpp"
#include <string>
#include <memory>
#include <iostream>

namespace smartcampus {

/**
 * @brief Abstract base class representing any physical building on campus.
 * Demonstrates:
 * - Data abstraction and encapsulation
 * - Virtual destructor for polymorphic deletion
 * - Pure virtual functions
 * - Static member variables and methods
 * - Disallowed copy semantics to avoid object slicing
 */
class Building {
private:
    std::string m_id;
    std::string m_name;
    BuildingCategory m_category;
    PriorityLevel m_priority;

    double m_essentialDemand;       // Non-deferrable demand (kWh)
    double m_discretionaryDemand;   // Flexible comfort/utility demand (kWh)
    double m_allocatedEnergy;       // Currently allocated energy in time slot (kWh)

    static int s_activeBuildingCount; // Tracks active building instances

protected:
    /**
     * @brief Protected constructor for derived classes.
     */
    Building(std::string id,
             std::string name,
             BuildingCategory category,
             PriorityLevel priority,
             double essentialDemand,
             double discretionaryDemand);

    // Protected setter for derived classes to manage allocation state
    void setAllocatedEnergy(double kwh);

public:
    virtual ~Building();

    // Disable copy constructor and copy assignment to prevent polymorphic slicing
    Building(const Building&) = delete;
    Building& operator=(const Building&) = delete;

    // Enable move semantics
    Building(Building&&) noexcept = default;
    Building& operator=(Building&&) noexcept = default;

    // Getters with const correctness
    const std::string& getId() const { return m_id; }
    const std::string& getName() const { return m_name; }
    BuildingCategory getCategory() const { return m_category; }
    PriorityLevel getPriority() const { return m_priority; }

    double getEssentialDemand() const { return m_essentialDemand; }
    double getDiscretionaryDemand() const { return m_discretionaryDemand; }
    double getTotalDemand() const { return m_essentialDemand + m_discretionaryDemand; }
    double getAllocatedEnergy() const { return m_allocatedEnergy; }
    double getUnmetDemand() const;

    FulfillmentStatus getFulfillmentStatus() const;
    EnergyAllocation getAllocationSummary() const;

    // Setters with validation
    void setDemands(double essential, double discretionary);
    void setPriority(PriorityLevel priority) { m_priority = priority; }

    // Pure virtual functions (Runtime Polymorphism / Abstraction)
    virtual std::string getTypeName() const = 0;
    virtual void allocate(double energyKwh) = 0;

    // Reset allocation for a new time slot
    virtual void resetAllocation();

    // Static member function
    static int getActiveBuildingCount() { return s_activeBuildingCount; }

    // Stream insertion operator overload
    friend std::ostream& operator<<(std::ostream& os, const Building& b);
};

} // namespace smartcampus

#endif // SMARTCAMPUS_BUILDING_HPP
