#include "smartcampus/educational/OOPDemonstration.hpp"
#include <iomanip>

namespace smartcampus::educational {

SmartGatewayHub::SmartGatewayHub(std::string gatewayCode, double calibrationVoltage)
    : m_gatewayCode(std::move(gatewayCode)),
      m_secretHardwareVoltage(calibrationVoltage) {}

std::string SmartGatewayHub::getAuditId() const {
    return "GATEWAY-" + m_gatewayCode;
}

void SmartGatewayHub::logAuditTrail(const std::string& action) {
    // Educational audit log simulation
    (void)action;
}

double SmartGatewayHub::readSensorTelemetry() const {
    return 240.0; // Simulated campus nominal voltage (Volts)
}

bool inspectInternalCalibration(const SmartGatewayHub& hub) {
    // Friend function accessing private member
    return hub.m_secretHardwareVoltage > 3.0 && hub.m_secretHardwareVoltage < 3.6;
}

void runEducationalConceptsDemo(std::ostream& os) {
    os << "===================================================================\n";
    os << " EDUCATIONAL OOP CONCEPTS DEMONSTRATION (SUPPLEMENTAL SYLLABUS)\n";
    os << "===================================================================\n";

    SmartGatewayHub hub("CAMPUS-NODE-01", 3.30);

    os << "[Concept: Interface Mixins / Multiple Inheritance]\n";
    const IAuditable* auditable = &hub;
    const IMonitored* monitored = &hub;
    os << "  -> Pointer to IAuditable: " << auditable->getAuditId() << "\n";
    os << "  -> Pointer to IMonitored: " << monitored->readSensorTelemetry() << " V\n";

    os << "[Concept: Friend Class & Friend Function]\n";
    double voltage = EnergyAuditor::inspectHardwareVoltage(hub);
    os << "  -> Friend Class EnergyAuditor read private calibration voltage: "
       << std::fixed << std::setprecision(2) << voltage << " V\n";
    bool valid = inspectInternalCalibration(hub);
    os << "  -> Friend Function inspectInternalCalibration returned: "
       << (valid ? "VALID (3.3V within bounds)" : "INVALID") << "\n";

    os << "===================================================================\n";
}

} // namespace smartcampus::educational
