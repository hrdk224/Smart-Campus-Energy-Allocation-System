#ifndef SMARTCAMPUS_OOP_DEMONSTRATION_HPP
#define SMARTCAMPUS_OOP_DEMONSTRATION_HPP

#include <string>
#include <iostream>

namespace smartcampus::educational {

/**
 * @file OOPDemonstration.hpp
 * @brief CLEARLY LABELLED EDUCATIONAL MODULE OUTSIDE CORE LOGIC.
 *
 * This module cleanly isolates supplemental syllabus concepts (such as multiple inheritance /
 * interface mixins and friend functions) without cluttering the clean single/multilevel
 * hierarchy of the core Smart Campus simulation.
 */

// 1. Interface Mixin 1
class IAuditable {
public:
    virtual ~IAuditable() = default;
    virtual std::string getAuditId() const = 0;
    virtual void logAuditTrail(const std::string& action) = 0;
};

// 2. Interface Mixin 2
class IMonitored {
public:
    virtual ~IMonitored() = default;
    virtual double readSensorTelemetry() const = 0;
};

// Forward declaration for friend function / class
class EnergyAuditor;

/**
 * @brief Demonstrates Multiple Inheritance (implementing multiple pure virtual interfaces),
 * explicit constructor conversion prevention, and friend class/function access.
 */
class SmartGatewayHub : public IAuditable, public IMonitored {
private:
    std::string m_gatewayCode;
    double m_secretHardwareVoltage; // Private internal hardware calibration

public:
    // Explicit constructor: prevents accidental implicit type conversion
    explicit SmartGatewayHub(std::string gatewayCode, double calibrationVoltage = 3.3);
    ~SmartGatewayHub() override = default;

    // Implementation of IAuditable
    std::string getAuditId() const override;
    void logAuditTrail(const std::string& action) override;

    // Implementation of IMonitored
    double readSensorTelemetry() const override;

    // Friend declaration: grants EnergyAuditor access to private m_secretHardwareVoltage
    friend class EnergyAuditor;
    friend bool inspectInternalCalibration(const SmartGatewayHub& hub);
};

/**
 * @brief Friend class accessing SmartGatewayHub's private state.
 */
class EnergyAuditor {
public:
    static double inspectHardwareVoltage(const SmartGatewayHub& hub) {
        return hub.m_secretHardwareVoltage;
    }
};

/**
 * @brief Friend function accessing private members of SmartGatewayHub.
 */
bool inspectInternalCalibration(const SmartGatewayHub& hub);

/**
 * @brief Runs an interactive or test-driven demonstration of all educational concepts.
 */
void runEducationalConceptsDemo(std::ostream& os = std::cout);

} // namespace smartcampus::educational

#endif // SMARTCAMPUS_OOP_DEMONSTRATION_HPP
