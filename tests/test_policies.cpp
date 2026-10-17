#include "TestHarness.hpp"
#include "smartcampus/policies/PriorityPolicy.hpp"
#include "smartcampus/policies/FairSharePolicy.hpp"
#include "smartcampus/models/AcademicBuilding.hpp"
#include "smartcampus/models/Laboratory.hpp"
#include "smartcampus/models/Hostel.hpp"

void runPolicyTests() {
    TEST_SUITE("Allocation Policy Algorithms and Tie-Breaking Tests");

    // Create 3 buildings for comparison
    // B1: Critical (P1), Essential: 30, Discretionary: 20 (Total: 50)
    smartcampus::Laboratory b1("BLD-1", "Lab 1", smartcampus::PriorityLevel::Critical, 30.0, 20.0, 2, true, 2, true);
    // B2: High (P2), Essential: 20, Discretionary: 20 (Total: 40)
    smartcampus::AcademicBuilding b2("BLD-2", "Academic 2", smartcampus::PriorityLevel::High, 20.0, 20.0, 4, false);
    // B3: Low (P4), Essential: 10, Discretionary: 20 (Total: 30)
    smartcampus::Hostel b3("BLD-3", "Hostel 3", smartcampus::PriorityLevel::Low, 10.0, 20.0, 100, 50, false);

    std::vector<smartcampus::Building*> buildings = {&b1, &b2, &b3};
    // Total Essential = 30 + 20 + 10 = 60 kWh
    // Total Discretionary = 20 + 20 + 20 = 60 kWh
    // Total Campus Demand = 120 kWh

    // Test 1: PriorityPolicy under moderate shortage
    // Supply = 80 kWh. Essential needs 60 kWh -> remaining = 20 kWh.
    // Discretionary: B1 (Critical) needs 20 kWh -> takes ALL 20 kWh!
    // B2 and B3 get 0 kWh discretionary!
    {
        smartcampus::PriorityPolicy policy;
        auto res = policy.allocate(buildings, 80.0);

        ASSERT_NEAR(res["BLD-1"].allocatedKwh, 50.0, 1e-4, "PriorityPolicy: B1 (Critical) gets 100% (50 kWh)");
        ASSERT_EQ(res["BLD-1"].status, smartcampus::FulfillmentStatus::FullyMet, "B1 is FullyMet");

        ASSERT_NEAR(res["BLD-2"].allocatedKwh, 20.0, 1e-4, "PriorityPolicy: B2 (High) gets essential only (20 kWh)");
        ASSERT_EQ(res["BLD-2"].status, smartcampus::FulfillmentStatus::PartiallyMet, "B2 is PartiallyMet");

        ASSERT_NEAR(res["BLD-3"].allocatedKwh, 10.0, 1e-4, "PriorityPolicy: B3 (Low) gets essential only (10 kWh)");
        ASSERT_EQ(res["BLD-3"].status, smartcampus::FulfillmentStatus::PartiallyMet, "B3 is PartiallyMet");
    }

    // Test 2: FairSharePolicy under same moderate shortage (80 kWh supply)
    // Essential needs 60 kWh -> satisfied for all. Remaining = 20 kWh.
    // Total discretionary = 60 kWh.
    // Each building has 20 kWh discretionary (equal share = 1/3 of remaining 20 kWh = 6.67 kWh each).
    // B1 gets 30 + 6.67 = 36.67 kWh.
    // B2 gets 20 + 6.67 = 26.67 kWh.
    // B3 gets 10 + 6.67 = 16.67 kWh. No building is starved of discretionary energy!
    {
        smartcampus::FairSharePolicy policy;
        auto res = policy.allocate(buildings, 80.0);

        ASSERT_NEAR(res["BLD-1"].allocatedKwh, 36.6667, 1e-2, "FairSharePolicy: B1 gets 30 ess + 6.67 disc");
        ASSERT_NEAR(res["BLD-2"].allocatedKwh, 26.6667, 1e-2, "FairSharePolicy: B2 gets 20 ess + 6.67 disc");
        ASSERT_NEAR(res["BLD-3"].allocatedKwh, 16.6667, 1e-2, "FairSharePolicy: B3 gets 10 ess + 6.67 disc (not starved)");

        double totalAlloc = res["BLD-1"].allocatedKwh + res["BLD-2"].allocatedKwh + res["BLD-3"].allocatedKwh;
        ASSERT_NEAR(totalAlloc, 80.0, 1e-4, "FairSharePolicy: Total allocated equals available supply 80 kWh");
    }

    // Test 3: Severe shortage under PriorityPolicy (Supply = 40 kWh < Essential 60 kWh)
    // B1 (Critical, 30 essential) gets 30 kWh. Remaining = 10 kWh.
    // B2 (High, 20 essential) gets remaining 10 kWh.
    // B3 (Low, 10 essential) gets 0 kWh (Deferred)!
    {
        smartcampus::PriorityPolicy policy;
        auto res = policy.allocate(buildings, 40.0);

        ASSERT_NEAR(res["BLD-1"].allocatedKwh, 30.0, 1e-4, "Severe Shortage Priority: B1 gets full 30 kWh essential");
        ASSERT_NEAR(res["BLD-2"].allocatedKwh, 10.0, 1e-4, "Severe Shortage Priority: B2 gets remaining 10 kWh essential");
        ASSERT_NEAR(res["BLD-3"].allocatedKwh, 0.0, 1e-4, "Severe Shortage Priority: B3 gets 0 kWh");
        ASSERT_EQ(res["BLD-3"].status, smartcampus::FulfillmentStatus::Deferred, "B3 is Deferred");
    }

    // Test 4: Severe shortage under FairSharePolicy (Supply = 40 kWh < Essential 60 kWh)
    // Scale = 40 / 60 = 2/3.
    // B1 gets 30 * 2/3 = 20 kWh.
    // B2 gets 20 * 2/3 = 13.33 kWh.
    // B3 gets 10 * 2/3 = 6.67 kWh.
    {
        smartcampus::FairSharePolicy policy;
        auto res = policy.allocate(buildings, 40.0);

        ASSERT_NEAR(res["BLD-1"].allocatedKwh, 20.0, 1e-2, "Severe Shortage FairShare: B1 gets proportional 20 kWh");
        ASSERT_NEAR(res["BLD-2"].allocatedKwh, 13.3333, 1e-2, "Severe Shortage FairShare: B2 gets proportional 13.33 kWh");
        ASSERT_NEAR(res["BLD-3"].allocatedKwh, 6.6667, 1e-2, "Severe Shortage FairShare: B3 gets proportional 6.67 kWh");
    }
}
