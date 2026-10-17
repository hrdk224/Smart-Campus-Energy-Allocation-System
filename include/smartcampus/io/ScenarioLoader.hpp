#ifndef SMARTCAMPUS_SCENARIO_LOADER_HPP
#define SMARTCAMPUS_SCENARIO_LOADER_HPP

#include "smartcampus/models/Campus.hpp"
#include "smartcampus/sources/EnergySupplyManager.hpp"
#include <string>
#include <memory>

namespace smartcampus {

/**
 * @brief Bundles a configured campus and its supply manager for a scenario.
 */
struct SimulationContext {
    std::unique_ptr<Campus> campus;
    std::unique_ptr<EnergySupplyManager> supplyManager;
    std::string scenarioName;
    std::string description;
};

/**
 * @brief Factory and file parser creating deterministic simulation scenarios.
 * Demonstrates:
 * - File I/O (CSV reading)
 * - Factory pattern
 * - Input validation and defensive programming
 */
class ScenarioLoader {
public:
    enum class PresetScenario {
        EnoughSupply,
        ModerateShortage,
        SevereShortage,
        SolarSurplus
    };

    /**
     * @brief Create a pre-configured built-in scenario.
     */
    static SimulationContext createPresetScenario(PresetScenario preset);

    /**
     * @brief Load a scenario from two CSV files: buildings and energy sources.
     */
    static SimulationContext loadFromCsv(const std::string& buildingsCsvPath,
                                         const std::string& sourcesCsvPath);

    /**
     * @brief Writes sample CSV scenario files to disk if they don't already exist.
     */
    static void generateDefaultScenarioFiles(const std::string& dataDirectory);
};

} // namespace smartcampus

#endif // SMARTCAMPUS_SCENARIO_LOADER_HPP
