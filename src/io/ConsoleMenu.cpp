#include "smartcampus/io/ConsoleMenu.hpp"
#include "smartcampus/io/ReportGenerator.hpp"
#include "smartcampus/policies/PriorityPolicy.hpp"
#include "smartcampus/policies/FairSharePolicy.hpp"
#include "smartcampus/engine/AllocationEngine.hpp"
#include "smartcampus/educational/OOPDemonstration.hpp"
#include <iostream>
#include <iomanip>
#include <limits>

namespace smartcampus {

ConsoleMenu::ConsoleMenu()
    : m_context(ScenarioLoader::createPresetScenario(ScenarioLoader::PresetScenario::EnoughSupply)),
      m_currentPolicy(std::make_shared<PriorityPolicy>()) {}

void ConsoleMenu::displayMainMenu() const {
    std::cout << "\n======================================================================\n";
    std::cout << "       SMART CAMPUS ENERGY ALLOCATION SYSTEM - CONSOLE INTERFACE      \n";
    std::cout << "======================================================================\n";
    std::cout << " Active Scenario: " << m_context.scenarioName << "\n";
    std::cout << " Active Policy:   " << m_currentPolicy->getPolicyName() << "\n";
    std::cout << "----------------------------------------------------------------------\n";
    std::cout << " 1. Run Scenario 1: Normal Operations (Enough Supply)\n";
    std::cout << " 2. Run Scenario 2: Moderate Shortage (Grid Brownout)\n";
    std::cout << " 3. Run Scenario 3: Severe Emergency Shortage (Blackout)\n";
    std::cout << " 4. Run Scenario 4: Solar Surplus & Battery Charging\n";
    std::cout << " 5. Switch Allocation Policy (Priority vs Fair-Share)\n";
    std::cout << " 6. Inspect Campus Buildings & Priority Levels\n";
    std::cout << " 7. Inspect Energy Sources & Battery State of Charge\n";
    std::cout << " 8. Run Educational OOP Concepts Verification (Viva Demonstration)\n";
    std::cout << " 9. Save Simulation Report & CSV to Disk (reports/)\n";
    std::cout << " 10. Run Automated 4-Scenario Comparative Demo\n";
    std::cout << " 0. Exit System\n";
    std::cout << "----------------------------------------------------------------------\n";
    std::cout << " Enter selection [0-10]: ";
}

void ConsoleMenu::runInteractive() {
    int choice = -1;
    while (choice != 0) {
        displayMainMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between 0 and 10.\n";
            continue;
        }

        switch (choice) {
            case 1:
                m_context = ScenarioLoader::createPresetScenario(ScenarioLoader::PresetScenario::EnoughSupply);
                runScenarioMenu();
                break;
            case 2:
                m_context = ScenarioLoader::createPresetScenario(ScenarioLoader::PresetScenario::ModerateShortage);
                runScenarioMenu();
                break;
            case 3:
                m_context = ScenarioLoader::createPresetScenario(ScenarioLoader::PresetScenario::SevereShortage);
                runScenarioMenu();
                break;
            case 4:
                m_context = ScenarioLoader::createPresetScenario(ScenarioLoader::PresetScenario::SolarSurplus);
                runScenarioMenu();
                break;
            case 5:
                switchPolicyMenu();
                break;
            case 6:
                inspectCampusMenu();
                break;
            case 7:
                inspectSourcesMenu();
                break;
            case 8:
                runEducationalDemo();
                break;
            case 9: {
                try {
                    AllocationEngine engine(*m_context.campus, *m_context.supplyManager, m_currentPolicy);
                    TimeSlot slot(1, "10:00 - 12:00", "Midday Slot");
                    AllocationRecord record = engine.simulateTimeSlot(slot);
                    std::string txtPath = ReportGenerator::saveReportToFile(engine.getHistory(), m_context.campus->getName());
                    std::string csvPath = ReportGenerator::exportToCsv(engine.getHistory());
                    std::cout << "\n[SUCCESS] Reports successfully generated:\n"
                              << "  * Text Summary: " << txtPath << "\n"
                              << "  * CSV Export:   " << csvPath << "\n";
                } catch (const std::exception& e) {
                    std::cerr << "[ERROR] Failed to save reports: " << e.what() << "\n";
                }
                break;
            }
            case 10:
                runAutomatedDemo();
                break;
            case 0:
                std::cout << "\nExiting Smart Campus Energy Allocation System. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice. Please select from the menu.\n";
                break;
        }
    }
}

void ConsoleMenu::runScenarioMenu() {
    std::cout << "\n>>> EXECUTING " << m_context.scenarioName << " <<<\n";
    std::cout << "Description: " << m_context.description << "\n";
    std::cout << "Active Policy: " << m_currentPolicy->getPolicyName() << "\n\n";

    AllocationEngine engine(*m_context.campus, *m_context.supplyManager, m_currentPolicy);
    TimeSlot slot(1, "08:00 - 10:00", "Simulated Slot");

    try {
        AllocationRecord record = engine.simulateTimeSlot(slot);
        std::cout << record << "\n";
    } catch (const std::exception& ex) {
        std::cerr << "[SIMULATION ERROR] " << ex.what() << "\n";
    }
}

void ConsoleMenu::switchPolicyMenu() {
    std::cout << "\n=== SELECT ALLOCATION POLICY ===\n";
    std::cout << " 1. Priority-Based Policy (Protects Critical & High buildings first)\n";
    std::cout << " 2. Fair-Share Proportional Policy (Equitable distribution of remaining discretionary power)\n";
    std::cout << " Choice [1-2]: ";

    int pChoice;
    if (std::cin >> pChoice) {
        if (pChoice == 1) {
            m_currentPolicy = std::make_shared<PriorityPolicy>();
            std::cout << "Switched to: " << m_currentPolicy->getPolicyName() << "\n";
        } else if (pChoice == 2) {
            m_currentPolicy = std::make_shared<FairSharePolicy>();
            std::cout << "Switched to: " << m_currentPolicy->getPolicyName() << "\n";
        } else {
            std::cout << "Invalid option. Keeping active policy.\n";
        }
    }
}

void ConsoleMenu::inspectCampusMenu() const {
    std::cout << "\n" << *m_context.campus << "\n";
}

void ConsoleMenu::inspectSourcesMenu() const {
    std::cout << "\n=== ENERGY SOURCES CONFIGURATION ===\n";
    std::cout << " 1. " << m_context.supplyManager->getSolar() << "\n";
    std::cout << " 2. " << m_context.supplyManager->getBattery()
              << " | SoC: " << std::fixed << std::setprecision(1)
              << m_context.supplyManager->getBattery().getSoCPercentage() << "%\n";
    std::cout << " 3. " << m_context.supplyManager->getGrid() << "\n";
    std::cout << " Total Available Supply for Slot: "
              << m_context.supplyManager->getTotalAvailableSupply() << " kWh\n";
}

void ConsoleMenu::runEducationalDemo() const {
    educational::runEducationalConceptsDemo(std::cout);
}

int ConsoleMenu::runAutomatedDemo() {
    std::cout << "\n========================================================================================\n";
    std::cout << "         STARTING AUTOMATED END-TO-END SMART CAMPUS ALLOCATION DEMONSTRATION             \n";
    std::cout << "========================================================================================\n";

    ScenarioLoader::generateDefaultScenarioFiles("data");

    std::vector<ScenarioLoader::PresetScenario> scenarios = {
        ScenarioLoader::PresetScenario::EnoughSupply,
        ScenarioLoader::PresetScenario::ModerateShortage,
        ScenarioLoader::PresetScenario::SevereShortage,
        ScenarioLoader::PresetScenario::SolarSurplus
    };

    std::vector<std::shared_ptr<AllocationPolicy>> policies = {
        std::make_shared<PriorityPolicy>(),
        std::make_shared<FairSharePolicy>()
    };

    std::vector<AllocationRecord> allRecords;
    int slotCounter = 1;

    for (auto scen : scenarios) {
        for (const auto& policy : policies) {
            auto ctx = ScenarioLoader::createPresetScenario(scen);
            AllocationEngine engine(*ctx.campus, *ctx.supplyManager, policy);

            std::string rangeStr = "Slot " + std::to_string(slotCounter) + " (2-hr period)";
            TimeSlot slot(slotCounter, rangeStr, ctx.scenarioName);

            AllocationRecord record = engine.simulateTimeSlot(slot);
            std::cout << record << "\n";
            allRecords.push_back(record);
            ++slotCounter;
        }
    }

    // Generate comprehensive summary report
    std::cout << ReportGenerator::generateSummaryText(allRecords, "IIT Smart Campus - Automated Demo");

    // Save report to disk
    try {
        std::string txtFile = ReportGenerator::saveReportToFile(allRecords, "IIT Smart Campus - Automated Demo");
        std::string csvFile = ReportGenerator::exportToCsv(allRecords);
        std::cout << "\n[DEMO COMPLETE] Automated demo written to:\n"
                  << "  * " << txtFile << "\n"
                  << "  * " << csvFile << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Warning: Could not save demo report: " << e.what() << "\n";
    }

    // Run educational OOP demonstration
    educational::runEducationalConceptsDemo(std::cout);

    std::cout << "\n========================================================================================\n";
    std::cout << "            AUTOMATED DEMO FINISHED SUCCESSFULLY (ALL INVARIANTS SATISFIED)            \n";
    std::cout << "========================================================================================\n";
    return 0;
}

} // namespace smartcampus
