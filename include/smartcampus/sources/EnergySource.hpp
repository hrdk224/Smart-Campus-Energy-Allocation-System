#ifndef SMARTCAMPUS_ENERGY_SOURCE_HPP
#define SMARTCAMPUS_ENERGY_SOURCE_HPP

#include <string>
#include <iostream>

namespace smartcampus {

/**
 * @brief Abstract base class for all campus power generation and storage units.
 * Demonstrates:
 * - Abstraction
 * - Virtual destructor
 * - Pure virtual functions
 * - Non-copyable polymorphic base
 */
class EnergySource {
private:
    std::string m_sourceId;
    std::string m_name;

protected:
    double m_capacityKwh;

    EnergySource(std::string sourceId, std::string name, double capacityKwh);

public:
    virtual ~EnergySource() = default;

    // Disallow copying to prevent object slicing
    EnergySource(const EnergySource&) = delete;
    EnergySource& operator=(const EnergySource&) = delete;

    // Allow moving
    EnergySource(EnergySource&&) noexcept = default;
    EnergySource& operator=(EnergySource&&) noexcept = default;

    const std::string& getId() const { return m_sourceId; }
    const std::string& getName() const { return m_name; }
    double getCapacityKwh() const { return m_capacityKwh; }

    // Pure virtual methods
    virtual double getAvailableEnergy() const = 0;
    virtual double supply(double requestedKwh) = 0;
    virtual std::string getSourceName() const = 0;

    // Virtual hook for periodic reset or replenishment
    virtual void resetForTimeSlot() {}

    friend std::ostream& operator<<(std::ostream& os, const EnergySource& source);
};

} // namespace smartcampus

#endif // SMARTCAMPUS_ENERGY_SOURCE_HPP
