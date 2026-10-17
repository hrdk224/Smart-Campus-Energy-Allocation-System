#ifndef SMARTCAMPUS_ENERGY_SOURCE_HPP
#define SMARTCAMPUS_ENERGY_SOURCE_HPP

#include <string>
#include <iostream>

namespace smartcampus {

/**
 * @brief Abstract base class for all campus power generation and storage units.
 *
 * KEY OOP CONCEPTS DEMONSTRATED:
 * 1. Abstraction: Defines pure virtual supply() and getAvailableEnergy()
 * 2. Virtual Destructor: Guarantees safe polymorphic cleanup
 * 3. Encapsulation: Protected state accessible only by derived sources
 */
class EnergySource {
private:
    std::string m_sourceId;
    std::string m_name;

protected:
    double m_capacityKwh;

    EnergySource(std::string sourceId, std::string name, double capacityKwh);

public:
    // =========================================================================
    // OOP CONCEPT: VIRTUAL DESTRUCTOR
    // =========================================================================
    virtual ~EnergySource() = default;

    // Disallow copying to prevent object slicing
    EnergySource(const EnergySource&) = delete;
    EnergySource& operator=(const EnergySource&) = delete;

    EnergySource(EnergySource&&) noexcept = default;
    EnergySource& operator=(EnergySource&&) noexcept = default;

    const std::string& getId() const { return m_sourceId; }
    const std::string& getName() const { return m_name; }
    double getCapacityKwh() const { return m_capacityKwh; }

    // =========================================================================
    // OOP CONCEPT: PURE VIRTUAL FUNCTIONS (ABSTRACT CONTRACT)
    // Concrete sources (Solar, Battery, Grid) MUST provide their own mechanics.
    // =========================================================================
    virtual double getAvailableEnergy() const = 0;
    virtual double supply(double requestedKwh) = 0;
    virtual std::string getSourceName() const = 0;

    virtual void resetForTimeSlot() {}

    friend std::ostream& operator<<(std::ostream& os, const EnergySource& source);
};

} // namespace smartcampus

#endif // SMARTCAMPUS_ENERGY_SOURCE_HPP
