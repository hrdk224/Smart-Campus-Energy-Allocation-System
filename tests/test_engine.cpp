#include "TestHarness.hpp"
#include "smartcampus/engine/AllocationEngine.hpp"
#include "smartcampus/policies/PriorityPolicy.hpp"
#include "smartcampus/policies/FairSharePolicy.hpp"
#include "smartcampus/io/ScenarioLoader.hpp"

void runEngineTests() {
    TEST_SUITE("Allocation Engine and Strategy Switching Tests");

    auto ctx = smartcampus::ScenarioLoader::createPresetScenario(
        smartcampus::ScenarioLoader::PresetScenario::ModerateShortage);

    auto priorityPolicy = std::make_shared<smartcampus::PriorityPolicy>();
    auto fairSharePolicy = std::make_shared<smartcampus::FairSharePolicy>();

    smartcampus::AllocationEngine engine(*ctx.campus, *ctx.supplyManager, priorityPolicy);
    ASSERT_EQ(engine.getPolicy().getPolicyName(), priorityPolicy->getPolicyName(), "Initial policy is PriorityPolicy");

    // Simulate slot 1 with PriorityPolicy
    smartcampus::TimeSlot slot1(1, "08:00 - 10:00", "Morning Rush");
    auto record1 = engine.simulateTimeSlot(slot1);

    ASSERT_EQ(record1.getTimeSlot().getSlotNumber(), 1, "Record 1 has slot 1");
    ASSERT_EQ(record1.getPolicyName(), priorityPolicy->getPolicyName(), "Record 1 used PriorityPolicy");
    ASSERT_TRUE(record1.getTotalAllocated() <= record1.getSupplySummary().totalAvailableKwh(), "Allocated <= Available");

    // Dynamic Policy Switching (Runtime Strategy Pattern)
    engine.setPolicy(fairSharePolicy);
    ASSERT_EQ(engine.getPolicy().getPolicyName(), fairSharePolicy->getPolicyName(), "Switched to FairSharePolicy");

    // Simulate slot 2 with FairSharePolicy
    smartcampus::TimeSlot slot2(2, "10:00 - 12:00", "Midday Lecture");
    auto record2 = engine.simulateTimeSlot(slot2);

    ASSERT_EQ(record2.getTimeSlot().getSlotNumber(), 2, "Record 2 has slot 2");
    ASSERT_EQ(record2.getPolicyName(), fairSharePolicy->getPolicyName(), "Record 2 used FairSharePolicy");

    // History contains both runs
    ASSERT_EQ(engine.getHistory().size(), 2, "Engine accumulated 2 simulation records in history");
}
