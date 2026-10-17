#include "TestHarness.hpp"
#include "smartcampus/models/AcademicBuilding.hpp"
#include "smartcampus/models/Laboratory.hpp"
#include "smartcampus/models/Hostel.hpp"
#include "smartcampus/models/AdministrativeBuilding.hpp"
#include "smartcampus/models/Campus.hpp"
#include "smartcampus/common/Exceptions.hpp"

void runBuildingTests() {
    TEST_SUITE("Building Hierarchy and Campus Model Tests");

    int initialCount = smartcampus::Building::getActiveBuildingCount();

    // Test 1: Laboratory construction & multilevel inheritance
    {
        auto lab = std::make_unique<smartcampus::Laboratory>(
            "LAB-TEST", "Robotics Lab", smartcampus::PriorityLevel::Critical,
            50.0, 30.0, 2, true, 4, true);

        ASSERT_EQ(lab->getId(), "LAB-TEST", "Lab ID matches");
        ASSERT_EQ(lab->getPriority(), smartcampus::PriorityLevel::Critical, "Lab priority is Critical");
        ASSERT_NEAR(lab->getTotalDemand(), 80.0, 1e-4, "Lab total demand = 50 + 30 = 80 kWh");
        ASSERT_EQ(lab->getFumeHoodCount(), 4, "Lab has 4 fume hoods");
        ASSERT_TRUE(lab->isBackupGeneratorRequired(), "Lab requires backup generator");

        // Allocation capping test
        lab->allocate(60.0);
        ASSERT_NEAR(lab->getAllocatedEnergy(), 60.0, 1e-4, "Allocated 60 kWh");
        ASSERT_NEAR(lab->getUnmetDemand(), 20.0, 1e-4, "Unmet demand is 20 kWh");
        ASSERT_EQ(lab->getFulfillmentStatus(), smartcampus::FulfillmentStatus::PartiallyMet, "Status is PartiallyMet");

        lab->allocate(100.0); // Exceeds total demand 80 kWh
        ASSERT_NEAR(lab->getAllocatedEnergy(), 80.0, 1e-4, "Allocation capped at total demand 80 kWh");
        ASSERT_NEAR(lab->getUnmetDemand(), 0.0, 1e-4, "Unmet demand is 0 kWh");
        ASSERT_EQ(lab->getFulfillmentStatus(), smartcampus::FulfillmentStatus::FullyMet, "Status is FullyMet");
    }

    // Test 2: Active building counter decrements after destruction
    ASSERT_EQ(smartcampus::Building::getActiveBuildingCount(), initialCount, "Building destructor decrements static count");

    // Test 3: Negative demand exception validation
    ASSERT_THROWS(
        smartcampus::AcademicBuilding("BAD-BLD", "Negative Essential", smartcampus::PriorityLevel::Medium, -10.0, 20.0, 5, false),
        smartcampus::NegativeEnergyException,
        "Reject negative essential demand"
    );

    ASSERT_THROWS(
        smartcampus::AcademicBuilding("BAD-BLD-2", "Negative Discretionary", smartcampus::PriorityLevel::Medium, 10.0, -20.0, 5, false),
        smartcampus::NegativeEnergyException,
        "Reject negative discretionary demand"
    );

    // Test 4: Campus aggregation and polymorphism
    smartcampus::Campus campus("Test IIT Campus");
    campus.addBuilding(std::make_unique<smartcampus::Laboratory>(
        "B1", "AI Lab", smartcampus::PriorityLevel::Critical, 40.0, 20.0, 2, true, 2, true));
    campus.addBuilding(std::make_unique<smartcampus::Hostel>(
        "B2", "Hostel 1", smartcampus::PriorityLevel::Medium, 30.0, 30.0, 200, 100, true));

    ASSERT_EQ(campus.getBuildingCount(), 2, "Campus has 2 buildings registered");
    ASSERT_NEAR(campus.getTotalEssentialDemand(), 70.0, 1e-4, "Total essential = 40 + 30 = 70 kWh");
    ASSERT_NEAR(campus.getTotalDiscretionaryDemand(), 50.0, 1e-4, "Total discretionary = 20 + 30 = 50 kWh");
    ASSERT_NEAR(campus.getTotalDemand(), 120.0, 1e-4, "Total demand = 120 kWh");

    auto* found = campus.findBuildingById("B1");
    ASSERT_TRUE(found != nullptr, "findBuildingById finds existing building");
    ASSERT_EQ(found->getName(), "AI Lab", "found building name matches");
    ASSERT_TRUE(campus.findBuildingById("NON-EXISTENT") == nullptr, "findBuildingById returns nullptr for unknown ID");
}
