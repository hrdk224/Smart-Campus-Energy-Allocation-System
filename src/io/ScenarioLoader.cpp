#include "smartcampus/io/ScenarioLoader.hpp"
#include "smartcampus/models/AcademicBuilding.hpp"
#include "smartcampus/models/Laboratory.hpp"
#include "smartcampus/models/Hostel.hpp"
#include "smartcampus/models/AdministrativeBuilding.hpp"
#include "smartcampus/common/Exceptions.hpp"
#include "smartcampus/utils/GenericUtils.hpp"
#include <fstream>
#include <sstream>

namespace smartcampus {

namespace {

std::unique_ptr<Campus> buildStandardCampus() {
    auto campus = std::make_unique<Campus>("Indian Institute of Technology & Science - Smart Campus");

    // Building 1: Specialized Research Laboratory (Critical P1)
    campus->addBuilding(std::make_unique<Laboratory>(
        "BLD-01", "Turing Computing & AI Lab", PriorityLevel::Critical,
        40.0, 30.0,  // Essential: 40 kWh, Discretionary: 30 kWh (Total 70)
        4, true,     // 4 rooms, smart boards
        8, true      // 8 fume hoods / server racks, backup required
    ));

    // Building 2: Major Academic Lecture Complex (High P2)
    campus->addBuilding(std::make_unique<AcademicBuilding>(
        "BLD-02", "Raman Academic Complex", PriorityLevel::High,
        35.0, 25.0,  // Essential: 35 kWh, Discretionary: 25 kWh (Total 60)
        12, true     // 12 classrooms, smart boards
    ));

    // Building 3: Central Library & Study Center (High P2)
    campus->addBuilding(std::make_unique<AdministrativeBuilding>(
        "BLD-03", "Aryabhatta Central Library", PriorityLevel::High,
        25.0, 25.0,  // Essential: 25 kWh, Discretionary: 25 kWh (Total 50)
        "Library Services", true // Has catalog server room
    ));

    // Building 4: Student Residential Hostel (Medium P3)
    campus->addBuilding(std::make_unique<Hostel>(
        "BLD-04", "Kalam Student Residence", PriorityLevel::Medium,
        30.0, 40.0,  // Essential: 30 kWh, Discretionary: 40 kWh (Total 70)
        250, 120, true // 250 residents, 120 rooms, dining hall
    ));

    // Building 5: Campus Administrative Secretariat (Low P4)
    campus->addBuilding(std::make_unique<AdministrativeBuilding>(
        "BLD-05", "Administrative Block", PriorityLevel::Low,
        15.0, 20.0,  // Essential: 15 kWh, Discretionary: 20 kWh (Total 35)
        "General Administration", false
    ));

    return campus;
}

} // anonymous namespace

SimulationContext ScenarioLoader::createPresetScenario(PresetScenario preset) {
    SimulationContext ctx;
    ctx.campus = buildStandardCampus();

    switch (preset) {
        case PresetScenario::EnoughSupply: {
            ctx.scenarioName = "Scenario 1: Normal Operations (Enough Supply)";
            ctx.description = "Full sunny day with ample solar and normal grid availability. "
                              "Campus demand is 285 kWh; total supply exceeds 550 kWh.";

            auto solar = std::make_unique<SolarSource>("SOL-01", "Rooftop Solar Array", 100.0, 0.85, 2.0); // 170 kWh
            auto battery = std::make_unique<BatterySource>("BAT-01", "Campus BESS Unit", 200.0, 120.0, 50.0, 50.0, 0.95, 0.10, 2.0);
            auto grid = std::make_unique<GridSource>("GRD-01", "Substation Main Feeder", 150.0, 1.0, 2.0); // 300 kWh

            ctx.supplyManager = std::make_unique<EnergySupplyManager>(
                std::move(solar), std::move(battery), std::move(grid));
            break;
        }

        case PresetScenario::ModerateShortage: {
            ctx.scenarioName = "Scenario 2: Moderate Shortage (Grid Brownout)";
            ctx.description = "Heavy overcast sky limits solar to 40 kWh. Municipal grid is restricted to 150 kWh. "
                              "Battery provides 20 kWh usable reserve. Total supply (210 kWh) satisfies all essential "
                              "demands (145 kWh), but discretionary demand (140 kWh) must be rationed.";

            auto solar = std::make_unique<SolarSource>("SOL-01", "Rooftop Solar Array", 100.0, 0.20, 2.0); // 40 kWh
            auto battery = std::make_unique<BatterySource>("BAT-01", "Campus BESS Unit", 200.0, 40.0, 50.0, 50.0, 0.95, 0.10, 2.0); // 20 kWh usable
            auto grid = std::make_unique<GridSource>("GRD-01", "Substation Main Feeder", 75.0, 1.0, 2.0); // 150 kWh

            ctx.supplyManager = std::make_unique<EnergySupplyManager>(
                std::move(solar), std::move(battery), std::move(grid));
            break;
        }

        case PresetScenario::SevereShortage: {
            ctx.scenarioName = "Scenario 3: Severe Emergency Shortage (Blackout)";
            ctx.description = "Night storm (zero solar), depleted battery (only 5 kWh usable), and regional blackout "
                              "(grid emergency feeder provides only 50 kWh). Total supply (55 kWh) is far below "
                              "campus essential demand (145 kWh). Triggers emergency life-safety rationing.";

            auto solar = std::make_unique<SolarSource>("SOL-01", "Rooftop Solar Array", 100.0, 0.0, 2.0); // 0 kWh
            auto battery = std::make_unique<BatterySource>("BAT-01", "Campus BESS Unit", 200.0, 25.0, 50.0, 50.0, 0.95, 0.10, 2.0); // 5 kWh usable
            auto grid = std::make_unique<GridSource>("GRD-01", "Substation Main Feeder", 25.0, 1.0, 2.0); // 50 kWh

            ctx.supplyManager = std::make_unique<EnergySupplyManager>(
                std::move(solar), std::move(battery), std::move(grid));
            break;
        }

        case PresetScenario::SolarSurplus: {
            ctx.scenarioName = "Scenario 4: Solar Surplus & Battery Charging";
            ctx.description = "Midday peak solar generation delivers 300 kWh. Campus demand is reduced to 180 kWh. "
                              "Solar alone covers 100% of campus loads. The remaining 120 kWh surplus is cleanly "
                              "routed into battery storage with zero grid dependency.";

            // Adjust building demands slightly for daytime low residential profile
            auto campus = std::make_unique<Campus>("Smart Green Campus - Daytime Solar Surplus");
            campus->addBuilding(std::make_unique<Laboratory>("BLD-01", "Turing Computing Lab", PriorityLevel::Critical, 40.0, 30.0, 4, true, 8, true));
            campus->addBuilding(std::make_unique<AcademicBuilding>("BLD-02", "Raman Academic Complex", PriorityLevel::High, 30.0, 20.0, 12, true));
            campus->addBuilding(std::make_unique<AdministrativeBuilding>("BLD-03", "Central Library", PriorityLevel::High, 20.0, 20.0, "Library", true));
            campus->addBuilding(std::make_unique<Hostel>("BLD-04", "Kalam Student Residence", PriorityLevel::Medium, 10.0, 15.0, 250, 120, false));
            campus->addBuilding(std::make_unique<AdministrativeBuilding>("BLD-05", "Administrative Block", PriorityLevel::Low, 10.0, 5.0, "Admin", false));
            ctx.campus = std::move(campus);

            auto solar = std::make_unique<SolarSource>("SOL-01", "Rooftop Solar Array", 150.0, 1.0, 2.0); // 300 kWh
            auto battery = std::make_unique<BatterySource>("BAT-01", "Campus BESS Unit", 200.0, 50.0, 75.0, 50.0, 0.95, 0.10, 2.0);
            auto grid = std::make_unique<GridSource>("GRD-01", "Substation Main Feeder", 100.0, 1.0, 2.0);

            ctx.supplyManager = std::make_unique<EnergySupplyManager>(
                std::move(solar), std::move(battery), std::move(grid));
            break;
        }
    }

    return ctx;
}

SimulationContext ScenarioLoader::loadFromCsv(const std::string& buildingsCsvPath,
                                              const std::string& sourcesCsvPath) {
    SimulationContext ctx;
    ctx.campus = std::make_unique<Campus>("CSV Imported Campus");
    ctx.scenarioName = "Custom CSV Scenario";
    ctx.description = "Imported from " + buildingsCsvPath + " and " + sourcesCsvPath;

    // Parse Buildings CSV
    std::ifstream bFile(buildingsCsvPath);
    if (!bFile.is_open()) {
        throw InvalidScenarioDataException("Failed to open buildings file: " + buildingsCsvPath);
    }

    std::string line;
    bool isFirstLine = true;
    while (std::getline(bFile, line)) {
        if (line.empty() || line[0] == '#') continue;
        if (isFirstLine) {
            isFirstLine = false;
            if (line.find("id") != std::string::npos || line.find("ID") != std::string::npos) {
                continue; // Skip header
            }
        }

        auto tokens = utils::split(line, ',');
        if (tokens.size() < 6) {
            continue;
        }

        std::string id = tokens[0];
        std::string name = tokens[1];
        std::string type = tokens[2];
        int priorityNum = std::stoi(tokens[3]);
        double essential = std::stod(tokens[4]);
        double discretionary = std::stod(tokens[5]);

        PriorityLevel priority = PriorityLevel::Medium;
        if (priorityNum == 1) priority = PriorityLevel::Critical;
        else if (priorityNum == 2) priority = PriorityLevel::High;
        else if (priorityNum == 3) priority = PriorityLevel::Medium;
        else priority = PriorityLevel::Low;

        if (type == "Laboratory") {
            ctx.campus->addBuilding(std::make_unique<Laboratory>(
                id, name, priority, essential, discretionary, 4, true, 4, true));
        } else if (type == "Hostel") {
            ctx.campus->addBuilding(std::make_unique<Hostel>(
                id, name, priority, essential, discretionary, 200, 100, true));
        } else if (type == "Administrative") {
            ctx.campus->addBuilding(std::make_unique<AdministrativeBuilding>(
                id, name, priority, essential, discretionary, "Admin", false));
        } else {
            ctx.campus->addBuilding(std::make_unique<AcademicBuilding>(
                id, name, priority, essential, discretionary, 8, true));
        }
    }
    bFile.close();

    // Parse Sources CSV
    std::ifstream sFile(sourcesCsvPath);
    if (!sFile.is_open()) {
        throw InvalidScenarioDataException("Failed to open sources file: " + sourcesCsvPath);
    }

    std::unique_ptr<SolarSource> solar;
    std::unique_ptr<BatterySource> battery;
    std::unique_ptr<GridSource> grid;

    isFirstLine = true;
    while (std::getline(sFile, line)) {
        if (line.empty() || line[0] == '#') continue;
        if (isFirstLine) {
            isFirstLine = false;
            if (line.find("type") != std::string::npos || line.find("Type") != std::string::npos) {
                continue;
            }
        }

        auto tokens = utils::split(line, ',');
        if (tokens.size() < 4) continue;

        std::string sourceType = tokens[0];
        std::string sourceId = tokens[1];
        std::string sourceName = tokens[2];

        if (sourceType == "Solar") {
            double peakKw = std::stod(tokens[3]);
            double factor = (tokens.size() > 4) ? std::stod(tokens[4]) : 1.0;
            solar = std::make_unique<SolarSource>(sourceId, sourceName, peakKw, factor, 2.0);
        } else if (sourceType == "Battery") {
            double cap = std::stod(tokens[3]);
            double soc = (tokens.size() > 4) ? std::stod(tokens[4]) : cap * 0.5;
            battery = std::make_unique<BatterySource>(sourceId, sourceName, cap, soc, 50.0, 50.0, 0.95, 0.10, 2.0);
        } else if (sourceType == "Grid") {
            double power = std::stod(tokens[3]);
            double avail = (tokens.size() > 4) ? std::stod(tokens[4]) : 1.0;
            grid = std::make_unique<GridSource>(sourceId, sourceName, power, avail, 2.0);
        }
    }
    sFile.close();

    if (!solar) solar = std::make_unique<SolarSource>("SOL-DEF", "Default Solar", 100.0, 0.5, 2.0);
    if (!battery) battery = std::make_unique<BatterySource>("BAT-DEF", "Default Battery", 200.0, 100.0, 50.0, 50.0, 0.95, 0.10, 2.0);
    if (!grid) grid = std::make_unique<GridSource>("GRD-DEF", "Default Grid", 150.0, 1.0, 2.0);

    ctx.supplyManager = std::make_unique<EnergySupplyManager>(
        std::move(solar), std::move(battery), std::move(grid));

    return ctx;
}

void ScenarioLoader::generateDefaultScenarioFiles(const std::string& dataDirectory) {
    // Generate campus buildings CSV
    std::string bldPath = dataDirectory + "/campus_buildings.csv";
    std::ofstream bldFile(bldPath);
    if (bldFile.is_open()) {
        bldFile << "# ID,Name,Type,Priority(1-4),EssentialDemandKwh,DiscretionaryDemandKwh\n";
        bldFile << "BLD-01,Turing Computing & AI Lab,Laboratory,1,40.0,30.0\n";
        bldFile << "BLD-02,Raman Academic Complex,Academic,2,35.0,25.0\n";
        bldFile << "BLD-03,Aryabhatta Central Library,Administrative,2,25.0,25.0\n";
        bldFile << "BLD-04,Kalam Student Residence,Hostel,3,30.0,40.0\n";
        bldFile << "BLD-05,Administrative Secretariat,Administrative,4,15.0,20.0\n";
        bldFile.close();
    }

    // Generate Scenario 1 Sources CSV (Enough Supply)
    std::ofstream s1(dataDirectory + "/sources_enough_supply.csv");
    if (s1.is_open()) {
        s1 << "# Type,ID,Name,CapacityOrPowerKw,FactorOrSoC\n";
        s1 << "Solar,SOL-01,Rooftop Solar Array,100.0,0.85\n";
        s1 << "Battery,BAT-01,Campus BESS Unit,200.0,120.0\n";
        s1 << "Grid,GRD-01,Substation Main Feeder,150.0,1.0\n";
        s1.close();
    }

    // Generate Scenario 2 Sources CSV (Moderate Shortage)
    std::ofstream s2(dataDirectory + "/sources_moderate_shortage.csv");
    if (s2.is_open()) {
        s2 << "# Type,ID,Name,CapacityOrPowerKw,FactorOrSoC\n";
        s2 << "Solar,SOL-01,Rooftop Solar Array,100.0,0.20\n";
        s2 << "Battery,BAT-01,Campus BESS Unit,200.0,40.0\n";
        s2 << "Grid,GRD-01,Substation Main Feeder,75.0,1.0\n";
        s2.close();
    }

    // Generate Scenario 3 Sources CSV (Severe Shortage)
    std::ofstream s3(dataDirectory + "/sources_severe_shortage.csv");
    if (s3.is_open()) {
        s3 << "# Type,ID,Name,CapacityOrPowerKw,FactorOrSoC\n";
        s3 << "Solar,SOL-01,Rooftop Solar Array,100.0,0.0\n";
        s3 << "Battery,BAT-01,Campus BESS Unit,200.0,25.0\n";
        s3 << "Grid,GRD-01,Substation Main Feeder,25.0,1.0\n";
        s3.close();
    }

    // Generate Scenario 4 Sources CSV (Solar Surplus)
    std::ofstream s4(dataDirectory + "/sources_solar_surplus.csv");
    if (s4.is_open()) {
        s4 << "# Type,ID,Name,CapacityOrPowerKw,FactorOrSoC\n";
        s4 << "Solar,SOL-01,Rooftop Solar Array,150.0,1.0\n";
        s4 << "Battery,BAT-01,Campus BESS Unit,200.0,50.0\n";
        s4 << "Grid,GRD-01,Substation Main Feeder,100.0,1.0\n";
        s4.close();
    }
}

} // namespace smartcampus
