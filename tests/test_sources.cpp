#include "TestHarness.hpp"
#include "smartcampus/sources/SolarSource.hpp"
#include "smartcampus/sources/BatterySource.hpp"
#include "smartcampus/sources/GridSource.hpp"
#include "smartcampus/sources/EnergySupplyManager.hpp"
#include "smartcampus/common/Exceptions.hpp"

void runSourceTests() {
    TEST_SUITE("Energy Sources and Supply Manager Tests");

    // Test 1: Solar generation
    {
        smartcampus::SolarSource solar("SOL-T", "Test Solar", 100.0, 0.5, 2.0);
        // 100 kW * 0.5 factor * 2.0 hours = 100 kWh
        ASSERT_NEAR(solar.getAvailableEnergy(), 100.0, 1e-4, "Solar generation available is 100 kWh");

        double supplied = solar.supply(60.0);
        ASSERT_NEAR(supplied, 60.0, 1e-4, "Solar supplied 60 kWh");
        ASSERT_NEAR(solar.getAvailableEnergy(), 40.0, 1e-4, "Solar has 40 kWh remaining");

        solar.resetForTimeSlot();
        ASSERT_NEAR(solar.getAvailableEnergy(), 100.0, 1e-4, "Solar resets available energy for new slot");
    }

    // Test 2: Battery invariants and charge/discharge
    {
        // 100 kWh capacity, 50 kWh initial SoC, 0.10 min reserve (10 kWh), 50 kW max discharge * 2h = 100 kWh
        smartcampus::BatterySource battery("BAT-T", "Test Battery", 100.0, 50.0, 50.0, 50.0, 0.90, 0.10, 2.0);

        ASSERT_NEAR(battery.getSoCPercentage(), 50.0, 1e-4, "Battery starts at 50% SoC");
        // Usable = 50 - 10 = 40 kWh
        ASSERT_NEAR(battery.getAvailableEnergy(), 40.0, 1e-4, "Battery usable energy is 40 kWh");

        double discharged = battery.supply(30.0);
        ASSERT_NEAR(discharged, 30.0, 1e-4, "Battery discharged 30 kWh");
        ASSERT_NEAR(battery.getCurrentSoC(), 20.0, 1e-4, "Battery remaining SoC is 20 kWh");
        ASSERT_NEAR(battery.getAvailableEnergy(), 10.0, 1e-4, "Battery usable energy is now 10 kWh (above 10 kWh reserve)");

        // Try to discharge 30 kWh when only 10 kWh usable
        double discharged2 = battery.supply(30.0);
        ASSERT_NEAR(discharged2, 10.0, 1e-4, "Battery stops at minimum reserve; supplies only 10 kWh");
        ASSERT_NEAR(battery.getCurrentSoC(), 10.0, 1e-4, "Battery at minimum reserve 10 kWh");
        ASSERT_NEAR(battery.getAvailableEnergy(), 0.0, 1e-4, "Battery available energy is 0 kWh");

        // Charging battery
        battery.resetForTimeSlot();
        double absorbed = battery.charge(20.0);
        ASSERT_NEAR(absorbed, 20.0, 1e-4, "Battery absorbed 20 kWh of surplus energy");
        // Headroom = 100 - 10 = 90 kWh. Efficiency = 0.90. Stored = 20 * 0.90 = 18 kWh.
        ASSERT_NEAR(battery.getCurrentSoC(), 28.0, 1e-4, "Battery SoC increased to 28 kWh after charging");
    }

    // Test 3: Battery bounds validation
    ASSERT_THROWS(
        smartcampus::BatterySource("BAT-BAD", "Impossible Battery", 100.0, 150.0),
        smartcampus::BatteryOverdrawException,
        "Reject initial SoC greater than capacity"
    );

    ASSERT_THROWS(
        smartcampus::BatterySource("BAT-BAD2", "Negative Capacity", -50.0, 10.0),
        smartcampus::CampusEnergyException,
        "Reject negative battery capacity"
    );

    // Test 4: EnergySupplyManager green-first dispatch
    {
        auto solar = std::make_unique<smartcampus::SolarSource>("SOL", "Solar", 50.0, 1.0, 2.0); // 100 kWh
        auto battery = std::make_unique<smartcampus::BatterySource>("BAT", "Battery", 100.0, 40.0, 50.0, 50.0, 0.95, 0.10, 2.0); // 30 kWh usable
        auto grid = std::make_unique<smartcampus::GridSource>("GRD", "Grid", 100.0, 1.0, 2.0); // 200 kWh

        smartcampus::EnergySupplyManager mgr(std::move(solar), std::move(battery), std::move(grid));
        ASSERT_NEAR(mgr.getTotalAvailableSupply(), 330.0, 1e-4, "Total available supply = 100 + 30 + 200 = 330 kWh");

        // Request 120 kWh: should consume 100 kWh solar, 20 kWh battery, 0 kWh grid
        auto summary = mgr.dispatchEnergy(120.0);
        ASSERT_NEAR(summary.solarUsedKwh, 100.0, 1e-4, "Solar fully utilized first (100 kWh)");
        ASSERT_NEAR(summary.batteryDischargedKwh, 20.0, 1e-4, "Battery provides remaining 20 kWh");
        ASSERT_NEAR(summary.gridUsedKwh, 0.0, 1e-4, "Grid not needed (0 kWh)");
        ASSERT_NEAR(summary.totalSuppliedKwh(), 120.0, 1e-4, "Total supplied matches requested 120 kWh");
    }
}
