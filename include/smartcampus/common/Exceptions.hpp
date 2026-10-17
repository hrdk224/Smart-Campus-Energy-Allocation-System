#ifndef SMARTCAMPUS_EXCEPTIONS_HPP
#define SMARTCAMPUS_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace smartcampus {

/**
 * @brief Base exception class for all Smart Campus domain errors.
 * Demonstrates inheritance from std::runtime_error.
 */
class CampusEnergyException : public std::runtime_error {
public:
    explicit CampusEnergyException(const std::string& message)
        : std::runtime_error("Campus Energy Error: " + message) {}
};

/**
 * @brief Thrown when negative energy values are provided.
 */
class NegativeEnergyException : public CampusEnergyException {
public:
    explicit NegativeEnergyException(const std::string& fieldName, double value)
        : CampusEnergyException("Negative energy rejected for [" + fieldName +
                                "]: " + std::to_string(value) + " kWh") {}
};

/**
 * @brief Thrown when battery invariants (SoC, capacity, charge/discharge rates) are violated.
 */
class BatteryOverdrawException : public CampusEnergyException {
public:
    explicit BatteryOverdrawException(const std::string& message)
        : CampusEnergyException("Battery Violation: " + message) {}
};

/**
 * @brief Thrown when total allocation exceeds available supply.
 */
class SupplyExceededException : public CampusEnergyException {
public:
    SupplyExceededException(double allocated, double available)
        : CampusEnergyException("Allocated energy (" + std::to_string(allocated) +
                                " kWh) strictly exceeds total available supply (" +
                                std::to_string(available) + " kWh)") {}
};

/**
 * @brief Thrown when input data/scenario files are corrupted or invalid.
 */
class InvalidScenarioDataException : public CampusEnergyException {
public:
    explicit InvalidScenarioDataException(const std::string& reason)
        : CampusEnergyException("Scenario Data Invalid: " + reason) {}
};

/**
 * @brief Thrown when policy configuration is invalid or missing.
 */
class PolicyException : public CampusEnergyException {
public:
    explicit PolicyException(const std::string& message)
        : CampusEnergyException("Allocation Policy Exception: " + message) {}
};

} // namespace smartcampus

#endif // SMARTCAMPUS_EXCEPTIONS_HPP
