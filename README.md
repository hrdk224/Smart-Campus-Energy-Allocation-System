# Smart Campus Energy Allocation System

[![C++17](https://img.shields.io/badge/Language-C%2B%2B17%20%2F%20C%2B%2B20-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Build & Tests](https://img.shields.io/badge/Tests-218%20Passing-brightgreen.svg)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Architecture](https://img.shields.io/badge/Architecture-Strategy%20%7C%20Polymorphism-purple.svg)](docs/ARCHITECTURE.md)

A robust, modular C++ Object-Oriented Programming (OOP) course project designed for a four-person team. The system simulates how a university campus distributes limited electrical energy across academic lecture halls, research laboratories, student hostels, libraries, and administrative complexes during discrete time intervals (e.g. 2-hour slots).

Energy is sourced dynamically from **Rooftop Solar Photovoltaic Arrays**, a **Campus Battery Energy Storage System (BESS)**, and the **Municipal Power Grid**. Distribution is governed by selectable policies (**Priority-Based** and **Fair-Share Proportional**) while maintaining physical conservation invariants and generating comprehensive audit reports.

---

## 1. Project Directory Layout

```
SmartCampusEnergyAllocation/
├── CMakeLists.txt                # CMake build configuration (C++17)
├── Makefile                      # One-command build/test/run wrapper
├── .gitignore                    # Build and artifact exclusions
├── README.md                     # Project overview and quickstart guide
├── include/                      # Public modular header interfaces
│   └── smartcampus/
│       ├── common/               # Domain enums, TimeSlot, and exceptions
│       │   ├── Exceptions.hpp    # Custom exception hierarchy
│       │   └── Types.hpp         # TimeSlot, EnergyAllocation, statuses
│       ├── models/               # Campus and building polymorphic hierarchy
│       │   ├── AcademicBuilding.hpp
│       │   ├── AdministrativeBuilding.hpp
│       │   ├── Building.hpp      # Abstract base class
│       │   ├── Campus.hpp        # Composition container
│       │   ├── Hostel.hpp        # Multilevel inheritance
│       │   ├── Laboratory.hpp    # Multilevel inheritance
│       │   └── ResidentialBuilding.hpp
│       ├── sources/              # Energy generation & storage hierarchy
│       │   ├── BatterySource.hpp # Storage invariants & SoC tracking
│       │   ├── EnergySource.hpp  # Abstract base class
│       │   ├── EnergySupplyManager.hpp # Green-first dispatch coordinator
│       │   ├── GridSource.hpp    # Grid backup feeder
│       │   └── SolarSource.hpp   # Intermittent solar PV
│       ├── policies/             # Strategy pattern allocation algorithms
│       │   ├── AllocationPolicy.hpp # Pure virtual strategy interface
│       │   ├── FairSharePolicy.hpp  # Proportional distribution
│       │   └── PriorityPolicy.hpp   # Two-stage priority ranking
│       ├── engine/               # Simulation orchestrator
│       │   ├── AllocationEngine.hpp # Invariant enforcement & workflow
│       │   └── AllocationRecord.hpp # Audit snapshot and stream formatting
│       ├── io/                   # Scenario loaders, UI, and report writers
│       │   ├── ConsoleMenu.hpp      # Interactive console interface
│       │   ├── ReportGenerator.hpp  # File I/O (text & CSV export)
│       │   └── ScenarioLoader.hpp   # Factory & CSV parser
│       ├── utils/                # Generic templates and helper functions
│       │   └── GenericUtils.hpp     # clampValue<T> & MetricAggregator<T>
│       └── educational/          # Supplemental syllabus concept isolation
│           └── OOPDemonstration.hpp # Multiple inheritance mixins & friend
├── src/                          # Implementation translation units
│   ├── common/
│   ├── models/
│   ├── sources/
│   ├── policies/
│   ├── engine/
│   ├── io/
│   ├── educational/
│   └── main.cpp                  # CLI entry point (--demo or interactive)
├── tests/                        # Zero-dependency automated test suite
│   ├── TestHarness.hpp           # Lightweight assertion runner
│   ├── test_buildings.cpp        # Hierarchy and demand tests
│   ├── test_sources.cpp          # Solar, battery SoC, and grid tests
│   ├── test_policies.cpp         # Priority vs FairShare algorithm tests
│   ├── test_engine.cpp           # Strategy switching and workflow tests
│   ├── test_invariants.cpp       # Conservation of energy tests
│   ├── test_educational_oop.cpp  # Supplemental syllabus tests
│   └── test_main.cpp             # Test executable entry point
├── data/                         # Sample deterministic CSV scenario datasets
│   ├── campus_buildings.csv
│   ├── sources_enough_supply.csv
│   ├── sources_moderate_shortage.csv
│   ├── sources_severe_shortage.csv
│   └── sources_solar_surplus.csv
├── docs/                         # Architectural and grading documentation
│   ├── ARCHITECTURE.md           # Rendered Mermaid class and flow diagrams
│   ├── OOP_CONCEPTS.md           # Complete concept-to-code viva table
│   └── TEAM_TASKS.md             # 4-student role assignment & git workflow
├── reports/                      # Generated simulation logs and CSV exports
│   └── .gitkeep
└── scripts/                      # Helper shell scripts
    ├── build.sh                  # Shell build script
    ├── run_demo.sh               # Non-interactive automated demo
    └── run_tests.sh              # Test runner script
```

---

## 2. Quickstart: Building & Running

### Prerequisites
- Modern C++ compiler supporting C++17 or C++20 (Apple Clang 12+, GCC 9+, or MSVC 2019+).
- CMake (version 3.16 or newer) or standard `make`.

### One-Command Build & Run with Makefile
```bash
# 1. Build the complete project and tests
make

# 2. Run all 218 automated unit and invariant test assertions
make test

# 3. Launch the non-interactive automated 4-scenario comparative demo
make demo

# 4. Launch the interactive console user interface
make run
```

### Manual Build using CMake directly
```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run tests
./build/campus_energy_tests

# Run interactive simulation
./build/campus_energy_sim

# Run automated demo
./build/campus_energy_sim --demo
```

---

## 3. Four Core Simulation Scenarios

The system includes 4 deterministic scenarios verifying edge cases and operational conditions:

| Scenario | Available Supply | Total Demand | Operating Conditions & Outcome |
| :--- | :---: | :---: | :--- |
| **1. Enough Supply** | 570.0 kWh | 285.0 kWh | Full midday sun + normal grid feeder. **100% of campus loads satisfied**; battery remains in healthy float. |
| **2. Moderate Shortage** | 210.0 kWh | 285.0 kWh | Overcast sky + grid brownout (capped at 150 kWh). **All essential loads (145 kWh) met**.<br>• *Priority Policy*: P1 & P2 receive discretionary power; P3 & P4 deferred.<br>• *Fair-Share Policy*: Discretionary power shared proportionally; no building starved. |
| **3. Severe Shortage** | 55.0 kWh | 285.0 kWh | Night storm (0 solar) + grid blackout (emergency line: 50 kWh). **Supply < essential baseline (145 kWh)**.<br>• Emergency rationing triggered.<br>• *Priority*: Critical labs protected first; lower buildings deferred.<br>• *Fair-Share*: Proportional essential load reduction. |
| **4. Solar Surplus** | 530.0 kWh | 200.0 kWh | High solar irradiance delivers 300 kWh directly.<br>• **100% solar powered** (0 kWh drawn from grid, 0 emissions).<br>• 100 kWh excess clean solar energy **routed into battery storage**, increasing SoC from 50 kWh to 145 kWh. |

---

## 4. Key Object-Oriented Concepts Demonstrated

See [`docs/OOP_CONCEPTS.md`](docs/OOP_CONCEPTS.md) for the exhaustive table containing exact classes, methods, and viva explanations. Highlights include:

- **Inheritance Hierarchies**:
  - Single: `Building` $\to$ `AcademicBuilding`, `Building` $\to$ `ResidentialBuilding`, `EnergySource` $\to$ `SolarSource`.
  - Multilevel: `Building` $\to$ `AcademicBuilding` $\to$ `Laboratory`, `Building` $\to$ `ResidentialBuilding` $\to$ `Hostel`.
- **Runtime Polymorphism**: Virtual destructors, pure virtual interfaces, `override` specifiers, and dynamic dispatch over `std::vector<Building*>`.
- **Strategy Design Pattern**: `AllocationEngine` associates with abstract `AllocationPolicy`, allowing dynamic runtime policy switching without engine modifications.
- **Composition & Smart Pointers**: `Campus` exclusively owns `Building` instances using `std::unique_ptr<Building>`, preventing memory leaks and avoiding raw `new`/`delete`.
- **Generic Templates**: `utils::clampValue<T>` function template and `utils::MetricAggregator<T>` class template for multi-slot energy statistical accumulation.
- **Operator Overloading**: `TimeSlot::operator<<`, `Building::operator<<`, and `EnergyAllocation::operator+` for natural domain syntax.
- **Exception Handling**: Custom exception hierarchy inheriting from `std::runtime_error` rejecting negative energy, invalid battery levels, and supply overdraws.

---

## 5. Four-Person Team Task Division

See [`docs/TEAM_TASKS.md`](docs/TEAM_TASKS.md) for individual files, inputs/outputs, peer reviews, and viva questions.

| Role | Lead | Domain & Responsibilities | Branch Name |
| :--- | :--- | :--- | :--- |
| **Role 1** | Student 1 | Campus & building polymorphic hierarchy, demand validation, static counters, registry. | `feature/campus-buildings` |
| **Role 2** | Student 2 | Solar, battery, grid sources, state of charge (SoC) tracking, green dispatch manager. | `feature/energy-sources` |
| **Role 3** | Student 3 | Allocation engine, Priority & Fair Share policies, invariant safety checks, record keeping. | `feature/allocation-engine` |
| **Role 4** | Student 4 *(Lead)* | Application CLI menu, CSV scenario loading, report generation, CMake, integration tests. | `feature/app-and-reporting` |

---

## 6. How to Publish This Repository to GitHub

If you wish to host this shared repository on GitHub for your project team, run these commands:

```bash
# Option 1: Authenticate with GitHub CLI and create remote repository
gh auth login
gh repo create smart-campus-energy-allocation --public --source=. --remote=origin --push

# Option 2: Push to an existing empty GitHub repository
git remote add origin https://github.com/<your-username>/smart-campus-energy-allocation.git
git branch -M main
git push -u origin main
```
