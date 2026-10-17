cmake_minimum_required(VERSION 3.16)
project(SmartCampusEnergyAllocation VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compiler warnings for high educational quality
if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Core library source files
set(CORE_SOURCES
    src/common/Types.cpp
    src/common/Exceptions.cpp
    src/models/Building.cpp
    src/models/AcademicBuilding.cpp
    src/models/Laboratory.cpp
    src/models/ResidentialBuilding.cpp
    src/models/Hostel.cpp
    src/models/AdministrativeBuilding.cpp
    src/models/Campus.cpp
    src/sources/EnergySource.cpp
    src/sources/SolarSource.cpp
    src/sources/BatterySource.cpp
    src/sources/GridSource.cpp
    src/sources/EnergySupplyManager.cpp
    src/policies/AllocationPolicy.cpp
    src/policies/PriorityPolicy.cpp
    src/policies/FairSharePolicy.cpp
    src/engine/AllocationEngine.cpp
    src/engine/AllocationRecord.cpp
    src/io/ScenarioLoader.cpp
    src/io/ReportGenerator.cpp
    src/io/ConsoleMenu.cpp
    src/educational/OOPDemonstration.cpp
)

# Core library
add_library(smart_campus_core STATIC ${CORE_SOURCES})
target_include_directories(smart_campus_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Main simulation executable
add_executable(campus_energy_sim src/main.cpp)
target_link_libraries(campus_energy_sim PRIVATE smart_campus_core)

# Test suite executable
enable_testing()
set(TEST_SOURCES
    tests/test_main.cpp
    tests/test_buildings.cpp
    tests/test_sources.cpp
    tests/test_engine.cpp
    tests/test_policies.cpp
    tests/test_invariants.cpp
    tests/test_educational_oop.cpp
)
add_executable(campus_energy_tests ${TEST_SOURCES})
target_link_libraries(campus_energy_tests PRIVATE smart_campus_core)

add_test(NAME AllCampusEnergyTests COMMAND campus_energy_tests)
