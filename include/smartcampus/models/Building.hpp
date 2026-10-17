#ifndef SMARTCAMPUS_BUILDING_HPP
#define SMARTCAMPUS_BUILDING_HPP

#include "smartcampus/common/Types.hpp"
#include <string>
#include <memory>
#include <iostream>

namespace smartcampus {

/**
 * @brief Abstract base class representing any physical building on campus.
 *
 * KEY OOP CONCEPTS DEMONSTRATED:
 * 1. Data Abstraction & Encapsulation (private fields, public getters/methods)
 * 2. Virtual Destructor (safe polymorphic cleanup)
 * 3. Pure Virtual Functions (defines an abstract interface)
 * 4. Static Members (shared active building counter)
 * 5. Const Correctness (immutability guarantees)
 * 6. Operator Overloading (stream insertion <<)
 */
class Building {
    // =========================================================================
    // OOP CONCEPT 1: ENCAPSULATION & DATA HIDING
    // Private variables cannot be accessed or corrupted from outside this class.
    // =========================================================================
private:
    std::string m_id;
    std::string m_name;
    BuildingCategory m_category;
    PriorityLevel m_priority;

    double m_essentialDemand;       // Non-deferrable critical demand (kWh)
    double m_discretionaryDemand;   // Flexible utility/comfort demand (kWh)
    double m_allocatedEnergy;       // Currently allocated energy in time slot (kWh)

    // =========================================================================
    // OOP CONCEPT 2: STATIC MEMBER VARIABLE
    // Shared by all instances of Building to track active object count.
    // =========================================================================
    static int s_activeBuildingCount;

protected:
    // Protected constructor: only derived classes can construct a Building
    Building(std::string id,
             std::string name,
             BuildingCategory category,
             PriorityLevel priority,
             double essentialDemand,
             double discretionaryDemand);

    // Protected setter: allows derived classes to safely update allocation
    void setAllocatedEnergy(double kwh);

public:
    // =========================================================================
    // OOP CONCEPT 3: VIRTUAL DESTRUCTOR
    // Essential for polymorphic base classes so derived objects delete cleanly.
    // =========================================================================
    virtual ~Building();

    // Prevent object slicing by disabling copy semantics on polymorphic base
    Building(const Building&) = delete;
    Building& operator=(const Building&) = delete;

    // Enable move semantics for efficient ownership transfers
    Building(Building&&) noexcept = default;
    Building& operator=(Building&&) noexcept = default;

    // =========================================================================
    // OOP CONCEPT 4: CONST CORRECTNESS
    // Guarantees getters do not mutate object state.
    // =========================================================================
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

    // Setter with input validation (throws NegativeEnergyException if negative)
    void setDemands(double essential, double discretionary);
    void setPriority(PriorityLevel priority) { m_priority = priority; }

    // =========================================================================
    // OOP CONCEPT 5: PURE VIRTUAL FUNCTIONS (ABSTRACTION)
    // Makes Building an abstract class; concrete derived classes MUST override.
    // =========================================================================
    virtual std::string getTypeName() const = 0;
    virtual void allocate(double energyKwh) = 0;

    virtual void resetAllocation();

    // Static member function accessing static counter
    static int getActiveBuildingCount() { return s_activeBuildingCount; }

    // =========================================================================
    // OOP CONCEPT 6: OPERATOR OVERLOADING
    // Overloading << provides clean, readable stream printing.
    // =========================================================================
    friend std::ostream& operator<<(std::ostream& os, const Building& b);
};

} // namespace smartcampus

#endif // SMARTCAMPUS_BUILDING_HPP
