#include "TestHarness.hpp"
#include "smartcampus/educational/OOPDemonstration.hpp"
#include "smartcampus/utils/GenericUtils.hpp"
#include "smartcampus/common/Types.hpp"
#include "smartcampus/common/Exceptions.hpp"

void runEducationalOOPTests() {
    TEST_SUITE("Educational OOP Concepts & Viva Verification Tests");

    // Test 1: Interface Mixins / Multiple Inheritance
    {
        smartcampus::educational::SmartGatewayHub hub("NODE-99", 3.30);
        smartcampus::educational::IAuditable* auditable = &hub;
        smartcampus::educational::IMonitored* monitored = &hub;

        ASSERT_EQ(auditable->getAuditId(), "GATEWAY-NODE-99", "Polymorphic call via IAuditable");
        ASSERT_NEAR(monitored->readSensorTelemetry(), 240.0, 1e-4, "Polymorphic call via IMonitored");
    }

    // Test 2: Friend class & friend function
    {
        smartcampus::educational::SmartGatewayHub hub("NODE-88", 3.30);
        double voltage = smartcampus::educational::EnergyAuditor::inspectHardwareVoltage(hub);
        ASSERT_NEAR(voltage, 3.30, 1e-4, "Friend class EnergyAuditor accessed private hardware voltage");

        bool isValid = smartcampus::educational::inspectInternalCalibration(hub);
        ASSERT_TRUE(isValid, "Friend function inspectInternalCalibration returned true for 3.3V");
    }

    // Test 3: Generic template functions & classes
    {
        // clampValue
        int clampedInt = smartcampus::utils::clampValue(15, 0, 10);
        ASSERT_EQ(clampedInt, 10, "clampValue correctly clamped int upper bound");

        double clampedDouble = smartcampus::utils::clampValue(-5.5, 0.0, 100.0);
        ASSERT_NEAR(clampedDouble, 0.0, 1e-4, "clampValue correctly clamped double lower bound");

        // MetricAggregator template
        smartcampus::utils::MetricAggregator<double> agg;
        agg.addSample(10.0);
        agg.addSample(20.0);
        agg.addSample(30.0);

        ASSERT_EQ(agg.count(), 3, "MetricAggregator sample count is 3");
        ASSERT_NEAR(agg.sum(), 60.0, 1e-4, "MetricAggregator sum is 60.0");
        ASSERT_NEAR(agg.average(), 20.0, 1e-4, "MetricAggregator average is 20.0");
        ASSERT_NEAR(agg.min(), 10.0, 1e-4, "MetricAggregator min is 10.0");
        ASSERT_NEAR(agg.max(), 30.0, 1e-4, "MetricAggregator max is 30.0");
    }

    // Test 4: Operator Overloading
    {
        smartcampus::EnergyAllocation a1;
        a1.requestedKwh = 50.0;
        a1.allocatedKwh = 30.0;
        a1.unmetKwh = 20.0;
        a1.status = smartcampus::FulfillmentStatus::PartiallyMet;

        smartcampus::EnergyAllocation a2;
        a2.requestedKwh = 40.0;
        a2.allocatedKwh = 40.0;
        a2.unmetKwh = 0.0;
        a2.status = smartcampus::FulfillmentStatus::FullyMet;

        smartcampus::EnergyAllocation aSum = a1 + a2;
        ASSERT_NEAR(aSum.requestedKwh, 90.0, 1e-4, "operator+ summed requested kWh");
        ASSERT_NEAR(aSum.allocatedKwh, 70.0, 1e-4, "operator+ summed allocated kWh");
        ASSERT_NEAR(aSum.unmetKwh, 20.0, 1e-4, "operator+ summed unmet kWh");
        ASSERT_EQ(aSum.status, smartcampus::FulfillmentStatus::PartiallyMet, "Compound status is PartiallyMet");
    }
}
