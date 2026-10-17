#ifndef SMARTCAMPUS_CONSOLE_MENU_HPP
#define SMARTCAMPUS_CONSOLE_MENU_HPP

#include "smartcampus/io/ScenarioLoader.hpp"
#include "smartcampus/policies/AllocationPolicy.hpp"
#include <memory>

namespace smartcampus {

/**
 * @brief Interactive terminal console interface for students and grading evaluators.
 * Provides interactive scenario execution, policy switching, report saving, and automated demo.
 */
class ConsoleMenu {
private:
    SimulationContext m_context;
    std::shared_ptr<AllocationPolicy> m_currentPolicy;

    void displayMainMenu() const;
    void runScenarioMenu();
    void switchPolicyMenu();
    void inspectCampusMenu() const;
    void inspectSourcesMenu() const;
    void runEducationalDemo() const;

public:
    ConsoleMenu();
    ~ConsoleMenu() = default;

    /**
     * @brief Launches the interactive console loop.
     */
    void runInteractive();

    /**
     * @brief Runs an automated non-interactive demonstration of all 4 scenarios with both policies.
     * Perfect for CI, automated testing, and unattended demos.
     */
    static int runAutomatedDemo();
};

} // namespace smartcampus

#endif // SMARTCAMPUS_CONSOLE_MENU_HPP
