# Team Task Distribution & Collaboration Workflow

This document details the responsibilities, assigned modules, inputs/outputs, Git branch workflow, acceptance criteria, and viva talking points for a four-person engineering team.

---

## 1. Team Roles & Task Matrix

| Role | Student / Lead | Assigned Files & Modules | Git Feature Branch | Peer Reviewer |
| :--- | :--- | :--- | :--- | :--- |
| **Role 1: Campus & Buildings** | Student 1 (Alex) | `include/smartcampus/models/*`<br>`src/models/*`<br>`tests/test_buildings.cpp` | `feature/campus-buildings` | Student 3 (Carlos) |
| **Role 2: Energy Sources & Storage** | Student 2 (Bianca) | `include/smartcampus/sources/*`<br>`src/sources/*`<br>`tests/test_sources.cpp` | `feature/energy-sources` | Student 4 (Diana) |
| **Role 3: Allocation Engine & Policies** | Student 3 (Carlos) | `include/smartcampus/policies/*`<br>`include/smartcampus/engine/*`<br>`src/policies/*`<br>`src/engine/*`<br>`tests/test_policies.cpp`<br>`tests/test_engine.cpp`<br>`tests/test_invariants.cpp` | `feature/allocation-engine` | Student 1 (Alex) |
| **Role 4: Application, I/O & Reporting** | Student 4 (Diana) *(Integration Lead)* | `include/smartcampus/io/*`<br>`src/io/*`<br>`src/main.cpp`<br>`data/*`<br>`docs/*`<br>`Makefile`, `CMakeLists.txt`<br>`scripts/*` | `feature/app-and-reporting` | Student 2 (Bianca) |

---

## 2. Shared Interfaces & First Agreement Contracts

Before starting parallel branch development, all four teammates agreed upon four central interface contracts in `include/smartcampus/common/`:

1. **`Types.hpp`**:
   - `BuildingCategory`, `PriorityLevel`, `FulfillmentStatus` enums.
   - `TimeSlot` class (comparison and streaming operators).
   - `EnergyAllocation` struct (containing `requestedKwh`, `allocatedKwh`, `unmetKwh`, `status`, `note`, and `operator+`).
   - `EnergySupplySummary` struct (holding solar, battery, grid dispatch quantities and SoC levels).
2. **`Exceptions.hpp`**:
   - Base `CampusEnergyException` and derived domain exceptions (`NegativeEnergyException`, `BatteryOverdrawException`, `SupplyExceededException`, `InvalidScenarioDataException`, `PolicyException`).
3. **`Building.hpp`** (Base interface for Role 1 and Role 3):
   - Contract for `Building::allocate(double energyKwh)` and getters (`getEssentialDemand()`, `getDiscretionaryDemand()`, `getPriority()`, `getId()`).
4. **`AllocationPolicy.hpp`** (Base interface for Role 3 and Role 4):
   - Pure virtual method `virtual std::map<std::string, EnergyAllocation> allocate(const std::vector<Building*>& buildings, double availableSupplyKwh) = 0;`.

---

## 3. Detailed Role Breakdown

### Role 1: Campus and Buildings Lead
- **Owner**: Student 1
- **Branch**: `feature/campus-buildings`
- **Assigned Files**:
  - `include/smartcampus/models/Building.hpp`
  - `include/smartcampus/models/AcademicBuilding.hpp`
  - `include/smartcampus/models/Laboratory.hpp`
  - `include/smartcampus/models/ResidentialBuilding.hpp`
  - `include/smartcampus/models/Hostel.hpp`
  - `include/smartcampus/models/AdministrativeBuilding.hpp`
  - `include/smartcampus/models/Campus.hpp`
  - `src/models/*.cpp`
  - `tests/test_buildings.cpp`
- **Inputs**: Building parameters (ID, name, priority level, essential and discretionary demands, specialized room counts).
- **Outputs**: Polymorphic building registry, demand calculations, building allocation states, active building instance counter.
- **Acceptance Criteria**:
  - `Building` base class is abstract with pure virtual functions (`getTypeName()`, `allocate()`) and virtual destructor.
  - Multilevel inheritance correctly demonstrated (`Building` $\to$ `AcademicBuilding` $\to$ `Laboratory`, `Building` $\to$ `ResidentialBuilding` $\to$ `Hostel`).
  - Negative demands reject with `NegativeEnergyException`.
  - Destructor decrements static active building counter.
  - All unit tests in `test_buildings.cpp` pass cleanly.
- **Viva Talking Points**:
  - Explain the difference between single and multilevel inheritance in your hierarchy.
  - Explain why `Building` copy constructor is `= delete`'d (to prevent object slicing).
  - Show how `Campus` uses `std::vector<std::unique_ptr<Building>>` to achieve composition and polymorphic ownership.

---

### Role 2: Energy Sources & Storage Lead
- **Owner**: Student 2
- **Branch**: `feature/energy-sources`
- **Assigned Files**:
  - `include/smartcampus/sources/EnergySource.hpp`
  - `include/smartcampus/sources/SolarSource.hpp`
  - `include/smartcampus/sources/BatterySource.hpp`
  - `include/smartcampus/sources/GridSource.hpp`
  - `include/smartcampus/sources/EnergySupplyManager.hpp`
  - `src/sources/*.cpp`
  - `tests/test_sources.cpp`
- **Inputs**: Solar irradiance factor, battery capacity, initial SoC, charge/discharge rates, round-trip efficiency, grid capacity and availability.
- **Outputs**: Available energy per source, executed dispatch kWh, battery state of charge (SoC) updates, surplus solar charging.
- **Acceptance Criteria**:
  - `EnergySource` base class provides pure virtual `supply(double)` and `getAvailableEnergy()`.
  - Solar generation scales proportionally with irradiance factor and slot duration.
  - Battery strictly preserves bounds: $0 \le \text{SoC} \le \text{Capacity}$.
  - Battery discharge halts at minimum health reserve (e.g. 10%).
  - Excess solar energy charges the battery up to headroom and rate limits.
  - All unit tests in `test_sources.cpp` pass cleanly.
- **Viva Talking Points**:
  - Explain how battery charge efficiency factor was applied.
  - Walk through how `EnergySupplyManager` implements green-first dispatch (Solar $\to$ Battery $\to$ Grid).
  - Show how impossible battery parameters throw domain exceptions.

---

### Role 3: Allocation Engine & Policies Lead
- **Owner**: Student 3
- **Branch**: `feature/allocation-engine`
- **Assigned Files**:
  - `include/smartcampus/policies/AllocationPolicy.hpp`
  - `include/smartcampus/policies/PriorityPolicy.hpp`
  - `include/smartcampus/policies/FairSharePolicy.hpp`
  - `include/smartcampus/engine/AllocationEngine.hpp`
  - `include/smartcampus/engine/AllocationRecord.hpp`
  - `src/policies/*.cpp`
  - `src/engine/*.cpp`
  - `tests/test_policies.cpp`, `tests/test_engine.cpp`, `tests/test_invariants.cpp`
- **Inputs**: `Building*` list, total available energy supply from `EnergySupplyManager`.
- **Outputs**: Computed `EnergyAllocation` map per building, total energy dispatched, unmet demand, fulfillment status (`FullyMet`, `PartiallyMet`, `Deferred`), invariant validation.
- **Acceptance Criteria**:
  - Strategy Pattern: `AllocationEngine` associates with `AllocationPolicy` via abstract interface; policy can be switched at runtime.
  - `PriorityPolicy`: Allocates essential demands first, then discretionary loads strictly in priority order (P1 down to P4), tie-broken deterministically by building ID.
  - `FairSharePolicy`: Protects essential baselines, then distributes remaining discretionary supply proportionally to prevent lower-priority starvation.
  - Severe shortage: If supply < essential demand, policies trigger emergency rationing and warnings.
  - Invariant: Allocated energy NEVER exceeds available supply.
  - All unit tests in `test_policies.cpp`, `test_engine.cpp`, and `test_invariants.cpp` pass cleanly.
- **Viva Talking Points**:
  - Explain the Strategy Pattern and why the engine doesn't need to know which concrete policy is active.
  - Compare the outcomes of Priority vs Fair Share during moderate shortage.
  - Explain the invariant assertion check `validateInvariants()`.

---

### Role 4: Application, I/O & Reporting Lead (Integration Lead)
- **Owner**: Student 4
- **Branch**: `feature/app-and-reporting`
- **Assigned Files**:
  - `include/smartcampus/io/ScenarioLoader.hpp`
  - `include/smartcampus/io/ReportGenerator.hpp`
  - `include/smartcampus/io/ConsoleMenu.hpp`
  - `include/smartcampus/utils/GenericUtils.hpp`
  - `include/smartcampus/educational/OOPDemonstration.hpp`
  - `src/io/*.cpp`, `src/educational/*.cpp`, `src/main.cpp`
  - `data/*.csv`, `reports/*`
  - `CMakeLists.txt`, `Makefile`, `scripts/*.sh`
  - `tests/test_educational_oop.cpp`, `tests/TestHarness.hpp`, `tests/test_main.cpp`
- **Inputs**: User terminal selections, CSV scenario datasets (`data/`).
- **Outputs**: Interactive console UI, non-interactive automated demo (`--demo`), formatted text and CSV simulation reports (`reports/`), clean build system.
- **Acceptance Criteria**:
  - Console menu supports all 4 standard scenarios, policy switching, inspection, and report export.
  - CSV files are loaded and parsed with defensive validation.
  - `ReportGenerator` utilizes generic template `MetricAggregator<T>` to calculate campus statistics.
  - Standalone automated demo passes end-to-end with zero errors.
  - Complete project builds cleanly under `-Wall -Wextra -Wpedantic`.
- **Viva Talking Points**:
  - Explain how templates (`clampValue<T>` and `MetricAggregator<T>`) promote code reuse.
  - Show how reports are written to disk with formatted tables and CSV export.
  - Demonstrate the `--demo` flag for automated continuous integration.

---

## 4. Integration & Merge Workflow

```
main (Base types, interfaces & CMake)
 │
 ├──> git checkout -b feature/campus-buildings     (Student 1) ───[PR #1 Review by S3]──┐
 │                                                                                       │
 ├──> git checkout -b feature/energy-sources      (Student 2) ───[PR #2 Review by S4]───┼──> Merge to main
 │                                                                                       │
 ├──> git checkout -b feature/allocation-engine   (Student 3) ───[PR #3 Review by S1]───┤
 │                                                                                       │
 └──> git checkout -b feature/app-and-reporting   (Student 4) ───[PR #4 Review by S2]───┘
```

### Conflict Prevention Checklist
1. **Never edit headers in another teammate's domain** without agreeing on interface changes in group chat first.
2. **Never push directly to `main`**: All features must go through a branch and Pull Request.
3. **Run `make test` before opening a PR**: Verify that all 218 test assertions pass locally.
4. **Cross-review requirement**: Each PR must receive an approving review from its assigned peer reviewer before merging.
5. **Merge Order**:
   1. `feature/campus-buildings` (Provides building models)
   2. `feature/energy-sources` (Provides source models)
   3. `feature/allocation-engine` (Integrates models into allocation algorithm)
   4. `feature/app-and-reporting` (Integrates everything into UI, I/O, and test suite)

---

## 5. Exact Git Commands for Team Members

### Publishing the Shared Repository to GitHub
*(Run by any student with an authenticated GitHub account)*:

```bash
# Option A: Using GitHub CLI (gh)
gh repo create smart-campus-energy-system --public --source=. --remote=origin --push

# Option B: Manual GitHub push (after creating empty repo on github.com)
git remote add origin https://github.com/<your-username>/smart-campus-energy-system.git
git branch -M main
git push -u origin main
```

### Developing on Feature Branches
```bash
# 1. Fetch latest main
git checkout main
git pull origin main

# 2. Create your assigned feature branch
git checkout -b feature/<your-assigned-branch>

# 3. Work on your files, compile, and run tests
make test

# 4. Commit and push your branch
git add include/ src/ tests/
git commit -m "feat(module): implement assigned role functionality"
git push -u origin feature/<your-assigned-branch>

# 5. Open a Pull Request on GitHub to merge into main
```
