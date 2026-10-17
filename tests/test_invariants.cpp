#include "TestHarness.hpp"
#include "smartcampus/io/ScenarioLoader.hpp"
#include "smartcampus/engine/AllocationEngine.hpp"
#include "smartcampus/policies/PriorityPolicy.hpp"
#include "smartcampus/policies/FairSharePolicy.hpp"
#include "smartcampus/common/Exceptions.hpp"

void runInvariantTests() {
    TEST_SUITE("Core Mathematical and Domain Invariant Tests");

    std::vector<smartcampus::ScenarioLoader::PresetScenario> scenarios = {
        smartcampus::ScenarioLoader::PresetScenario::EnoughSupply,
        smartcampus::ScenarioLoader::PresetScenario::ModerateShortage,
        smartcampus::ScenarioLoader::PresetScenario::SevereShortage,
        smartcampus::ScenarioLoader::PresetScenario::SolarSurplus
    };

    std::vector<std::shared_ptr<smartcampus::AllocationPolicy>> policies = {
        std::make_shared<smartcampus::PriorityPolicy>(),
        std::make_shared<smartcampus::FairSharePolicy>()
    };

    int scenarioIdx = 0;
    for (auto scen : scenarios) {
        ++scenarioIdx;
        for (const auto& policy : policies) {
            auto ctx = smartcampus::ScenarioLoader::createPresetScenario(scen);
            smartcampus::AllocationEngine engine(*ctx.campus, *ctx.supplyManager, policy);
            smartcampus::TimeSlot slot(1, "08:00 - 10:00", "Invariant Test");

            auto record = engine.simulateTimeSlot(slot);

            // Invariant 1: Allocated energy NEVER exceeds total available supply
            double totalAvailable = record.getSupplySummary().totalAvailableKwh();
            ASSERT_TRUE(record.getTotalAllocated() <= totalAvailable + 1e-4,
                "Invariant 1 (Scenario " + std::to_string(scenarioIdx) + ", " + policy->getPolicyName() +
                "): Allocated (" + std::to_string(record.getTotalAllocated()) + ") <= Supply (" +
                std::to_string(totalAvailable) + ")");

            // Invariant 2: No allocation is negative
            for (const auto& snap : record.getSnapshots()) {
                ASSERT_TRUE(snap.allocation.allocatedKwh >= 0.0,
                    "Invariant 2: Building " + snap.buildingId + " allocation is non-negative");

                // Invariant 3: Unmet demand is calculated correctly
                double expectedUnmet = std::max(0.0, snap.allocation.requestedKwh - snap.allocation.allocatedKwh);
                ASSERT_NEAR(snap.allocation.unmetKwh, expectedUnmet, 1e-4,
                    "Invariant 3: Building " + snap.buildingId + " unmet demand matches requested - allocated");

                // Invariant 4: Allocation never exceeds requested demand
                ASSERT_TRUE(snap.allocation.allocatedKwh <= snap.allocation.requestedKwh + 1e-4,
                    "Invariant 4: Building " + snap.buildingId + " allocation does not exceed requested");
            }

            // Invariant 5: Battery capacity and limits respected
            double batteryCap = ctx.supplyManager->getBattery().getCapacityKwh();
            double batterySoC = ctx.supplyManager->getBattery().getCurrentSoC();
            ASSERT_TRUE(batterySoC >= 0.0 && batterySoC <= batteryCap + 1e-4,
                "Invariant 5: Battery SoC (" + std::to_string(batterySoC) + ") within [0, " +
                std::to_string(batteryCap) + "] kWh");
        }
    }
}
