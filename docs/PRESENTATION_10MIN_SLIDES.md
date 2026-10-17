# 10-Minute B.Tech Project Presentation Guide & Slide Deck

> **Project Title:** Smart Campus Energy Allocation System  
> **Course:** Object-Oriented Programming (C++)  
> **Team Size:** 4 Students  
> **Target Duration:** 10 Minutes Total (8 mins slides + 2 mins live demo & viva)

---

## ⏱️ Presentation Timing Plan (10 Minutes)

| Slide | Topic | Presenter | Time |
| :---: | :--- | :--- | :---: |
| **1** | Title, Objective & Team Distribution | Student 4 (Integration Lead) | 0:45 min |
| **2** | Problem Statement & Real-World Context | Student 4 | 0:45 min |
| **3** | High-Level Architecture & Class Design | Student 1 (Campus & Buildings) | 1:00 min |
| **4** | Building Hierarchy (Single & Multilevel Inheritance) | Student 1 | 1:15 min |
| **5** | Energy Sources & Battery Management (SoC & Invariants) | Student 2 (Energy Systems) | 1:15 min |
| **6** | Strategy Pattern: Allocation Policies (Priority vs Fair-Share) | Student 3 (Engine & Algorithms) | 1:30 min |
| **7** | Invariant Safety, Conservation & Defensive OOP | Student 3 | 1:00 min |
| **8** | Modern C++ Features (Smart Pointers, Templates, Operator Overloads) | Student 4 | 1:00 min |
| **9** | **Live Terminal Demonstration** (`make demo`) | All Team Members | 1:30 min |
| **10** | Conclusion, Deliverables & Viva Q&A | All Team Members | 1:00 min |

---

## 🖥️ Slide-by-Slide Content & Speaker Script

---

### Slide 1: Title & Team Distribution
**Slide Title:** Smart Campus Energy Allocation System  
**Subtitle:** A Modular Object-Oriented Simulation in Modern C++17

**Slide Content:**
- **Project Goal:** Simulate intelligent, policy-driven electricity distribution across university facilities using Solar, Battery (BESS), and Power Grid sources.
- **Team Roles & Branch Ownership:**
  - **Student 1 (Alex):** Campus & Buildings Hierarchy (`feature/campus-buildings`)
  - **Student 2 (Bianca):** Energy Generation & Battery Storage (`feature/energy-sources`)
  - **Student 3 (Carlos):** Allocation Engine & Strategy Policies (`feature/allocation-engine`)
  - **Student 4 (Diana - Lead):** UI, CSV I/O, Reports & Integration (`feature/app-and-reporting`)

🗣️ **Speaker Script (Student 4):**
> *"Good morning respected professors and evaluators. Today, our four-person team presents the Smart Campus Energy Allocation System. In modern educational institutions, managing power outages and renewable generation is a critical operational challenge. We built a modular, production-quality simulation in C++17 that distributes energy across labs, classrooms, hostels, and offices according to essential baselines and selectable policies."*

---

### Slide 2: Problem Statement & Campus Model
**Slide Title:** Problem Statement: The Microgrid Allocation Challenge

**Slide Content:**
- **The Challenge:**
  - Limited and intermittent power supply (cloud cover reducing solar, grid brownouts).
  - Heterogeneous consumers with varying criticality:
    - **P1 - Critical (Laboratories):** Life-safety fume hoods, sensitive servers.
    - **P2 - High (Academic Complex, Library):** Scheduled lectures, study halls.
    - **P3 - Medium (Hostels):** Evening dining mess, resident lighting.
    - **P4 - Low (Administrative Block):** Discretionary office plug loads.
- **The Objective:**
  - Maximize renewable solar utilization.
  - Guarantee non-negotiable essential baselines.
  - Dynamically adapt allocation under severe shortages and solar surpluses.

🗣️ **Speaker Script (Student 4):**
> *"A campus is not a uniform electrical load. A computer science server lab or chemistry lab has non-negotiable safety requirements, whereas administrative air conditioning can be temporarily deferred. Our simulation discretizes time into discrete slots, evaluates demands into essential and discretionary tiers, and allocates available supply while preventing battery over-discharge."*

---

### Slide 3: OOP Architecture Overview
**Slide Title:** Object-Oriented System Architecture

**Slide Content:**
*(Embed Mermaid Diagram from `docs/ARCHITECTURE.md` or 3 core boxes)*
- **Model Layer:** `Building` abstract base with concrete derived types.
- **Source Layer:** `EnergySource` abstract base with `SolarSource`, `BatterySource`, `GridSource`.
- **Engine Layer:** `AllocationEngine` working through `AllocationPolicy` interface (Strategy Pattern).
- **Ownership:** `Campus` owns `std::vector<std::unique_ptr<Building>>` (Composition via RAII).

🗣️ **Speaker Script (Student 1):**
> *"To ensure all four teammates could build independently without merge conflicts, we designed a decoupled architecture. We established shared interfaces first. Notice that the Allocation Engine does not depend on concrete policies—it communicates purely through an abstract `AllocationPolicy` interface. Memory is managed exclusively through standard smart pointers, eliminating raw memory leaks."*

---

### Slide 4: Building Hierarchy (Inheritance & Polymorphism)
**Slide Title:** Campus & Buildings: Inheritance in Action

**Slide Content:**
- **Single Inheritance:**
  - `Building` $\to$ `AcademicBuilding` (adds classrooms, smartboards)
  - `Building` $\to$ `ResidentialBuilding` (adds resident capacity)
- **Multilevel Inheritance:**
  - `Building` $\to$ `AcademicBuilding` $\to$ `Laboratory` (adds fume hoods & critical safety baseline)
  - `Building` $\to$ `ResidentialBuilding` $\to$ `Hostel` (adds dining mess kitchen load)

**Code Snippet for Slide:**
```cpp
// Multilevel Inheritance: Building -> AcademicBuilding -> Laboratory
class Laboratory : public AcademicBuilding {
private:
    int m_hazardousFumeHoods;
    double m_safetyCriticalBaselineKwh;
public:
    std::string getTypeName() const override { 
        return "Specialized Research Laboratory"; 
    }
    void allocate(double energyKwh) override {
        setAllocatedEnergy(std::min(energyKwh, getTotalDemand()));
    }
};
```

🗣️ **Speaker Script (Student 1):**
> *"I was responsible for Module 1. We utilized both single and multilevel inheritance. For example, a `Laboratory` is an `AcademicBuilding`, which in turn is a `Building`. It inherits identity and demand tracking from `Building`, classroom management from `AcademicBuilding`, and adds hazardous fume hood baselines. We enforced encapsulation: demand fields are strictly private, and allocating energy polymorphically clamps to total demand."*

---

### Slide 5: Energy Sources & Battery Invariants
**Slide Title:** Energy Sources: Dispatch & Battery Storage

**Slide Content:**
- **Green-First Dispatch Priority:**
  1. **Solar PV:** Zero marginal emissions; dispatched first.
  2. **Battery Storage (BESS):** Discharges stored clean energy up to power limits.
  3. **Municipal Grid:** Supplemental backup or off-peak feeder.
- **Battery Invariant Maintenance:**
  - Bound: $0 \le \text{Current SoC} \le \text{Capacity}$
  - Health Protection: Discharge halts at 10% reserve threshold.
  - Solar Surplus: Excess solar ($\text{Solar} > \text{Demand}$) automatically charges the battery with 95% round-trip efficiency!

**Code Snippet for Slide:**
```cpp
// Defensive battery invariant: discharge cannot breach reserve
double BatterySource::getAvailableEnergy() const {
    double usableSoC = std::max(0.0, m_currentSoCKwh - getMinReserveKwh());
    double maxSlotDischarge = m_maxDischargeRateKw * m_slotDurationHours;
    double remainingLimit = std::max(0.0, maxSlotDischarge - m_dischargedInSlotKwh);
    return std::min(usableSoC, remainingLimit);
}
```

🗣️ **Speaker Script (Student 2):**
> *"In Module 2, I developed the physical energy sources derived from the abstract base class `EnergySource`. Our campus battery is not just a passive number: it tracks state-of-charge, caps instantaneous charge and discharge power ratings, and halts discharge when the 10% reserve limit is reached. In midday sunny scenarios, any solar energy exceeding campus demand is routed to recharge the battery."*

---

### Slide 6: Strategy Pattern: Allocation Policies
**Slide Title:** Strategy Pattern: Priority vs. Fair-Share Policies

**Slide Content:**
- **The Interface:** `AllocationPolicy` defines `virtual allocate(...) = 0;`
- **Policy 1: Priority-Based Policy:**
  - Stage 1: Satisfies Essential loads by priority (P1 down to P4).
  - Stage 2: Distributes remaining supply to Discretionary loads in strict priority order.
  - Equal priority tie-break: Deterministic by Building ID.
- **Policy 2: Fair-Share Proportional Policy:**
  - Stage 1: Protects essential baselines for all buildings.
  - Stage 2: Distributes discretionary power **proportionally** based on requested load ratio.
  - **Key Benefit:** Prevents starvation of hostels and administrative offices during moderate shortages!

**Comparison Table for Slide:**
| Scenario: Moderate Shortage (210 kWh Supply vs 285 kWh Demand) | Priority Policy | Fair-Share Policy |
| :--- | :---: | :---: |
| **P1 - Lab (70 kWh requested)** | **70.0 kWh (100% Met)** | 53.9 kWh (Partially Met) |
| **P2 - Academic (60 kWh requested)** | **60.0 kWh (100% Met)** | 46.6 kWh (Partially Met) |
| **P3 - Hostel (70 kWh requested)** | 30.0 kWh (Essential only) | **48.6 kWh (Shared Discretionary)** |
| **P4 - Admin (35 kWh requested)** | 15.0 kWh (Essential only) | **24.3 kWh (Shared Discretionary)** |

🗣️ **Speaker Script (Student 3):**
> *"I designed the Allocation Engine and policies. By applying the Strategy Design Pattern, our engine can switch policies on the fly at runtime. Look at this comparison table: under a moderate grid brownout, Priority Policy gives 100% comfort power to labs and classrooms, leaving the hostel with only essential lighting. But Fair-Share guarantees essential power to everyone, then divides the remaining discretionary electricity proportionally so no facility is completely starved."*

---

### Slide 7: Invariant Verification & Defensive OOP
**Slide Title:** Mathematical Invariants & Exception Safety

**Slide Content:**
- **Physical Conservation of Energy:**
  $$\sum_{i} \text{Allocated}_i \le \text{Total Available Supply}$$
- **Non-Negativity Guarantee:** $\forall i, \text{Allocated}_i \ge 0$
- **Unmet Demand Invariant:** $\text{Unmet}_i = \text{Requested}_i - \text{Allocated}_i$
- **Custom Exception Hierarchy:**
  - `CampusEnergyException` (inherits from `std::runtime_error`)
  - `NegativeEnergyException`
  - `SupplyExceededException`
  - `BatteryOverdrawException`

🗣️ **Speaker Script (Student 3):**
> *"In academic projects, robustness is paramount. Our engine has an internal `validateInvariants()` method that mathematically asserts that energy allocated never exceeds energy supplied, no negative numbers can exist, and unmet demand is exact. If a corrupted scenario file is fed into the system, our custom exception hierarchy catches it cleanly."*

---

### Slide 8: Modern C++ Techniques & Tooling
**Slide Title:** Modern C++ Best Practices & Generic Templates

**Slide Content:**
- **RAII & Smart Pointers:** Exclusive `std::unique_ptr` in `Campus`; shared references in `AllocationEngine`. Zero raw `new` / `delete`.
- **Generic Templates:**
  - `clampValue<T>(val, min, max)`: Reusable boundary enforcement.
  - `MetricAggregator<T>`: Generic statistical accumulator for sum, average, min, and max energy metrics across time slots.
- **Operator Overloading:** Stream insertion (`operator<<`) and compound addition (`operator+`).
- **Standardized Build System:** Portable CMakeLists.txt and top-level Makefile.

🗣️ **Speaker Script (Student 4):**
> *"In Module 4, I integrated the components, created the CSV scenario loader, built the console UI, and wrote our automated test harness. We leveraged generic templates like `MetricAggregator` to compute campus-wide statistical averages, overloaded `operator<<` for clean table formatting, and provided CSV export so data can be opened directly in Excel."*

---

### Slide 9: Live Terminal Demonstration
**Slide Title:** Live Demonstration & Test Verification

**Steps to Execute on Terminal (takes 30 seconds):**
1. Run test suite:
   ```bash
   make test
   ```
   *(Show: 218 test assertions passing with 0 failures!)*
2. Run automated demo:
   ```bash
   make demo
   ```
   *(Show: Side-by-side execution of all 4 scenarios with both Priority and Fair-Share policies!)*

🗣️ **Speaker Script (All):**
> *"Now let us demonstrate the live software. Running `make test` executes our 218 unit and invariant test cases covering every class and edge case. Notice that 100% of tests pass cleanly with zero compiler warnings under `-Wall -Wextra -Wpedantic`. Next, running `make demo` simulates our four test scenarios—Normal Sun, Grid Brownout, Blackout, and Solar Surplus—generating full audit reports."*

---

### Slide 10: Conclusion & Viva Cheat Sheet
**Slide Title:** Conclusion & Summary of Deliverables

**Slide Content:**
- ✅ **Fully Working Simulation:** Tested, deterministic, zero external dependencies.
- ✅ **Clean 4-Person Collaboration:** Clear Git branches, peer-reviewed pull requests.
- ✅ **Comprehensive Documentation:** Full architectural diagrams (`docs/ARCHITECTURE.md`) and OOP concept viva mapping (`docs/OOP_CONCEPTS.md`).

---

## 🎓 Top 8 Viva Questions & Exact 1-Sentence Answers

Be prepared for the examiner to ask any of the following questions during your viva:

1. **Q: Where is runtime polymorphism implemented in your project?**  
   *A:* "In `AllocationPolicy.hpp` line 31, where the pure virtual method `virtual map<string, EnergyAllocation> allocate(...) = 0;` is dynamically dispatched at runtime when `AllocationEngine` executes `m_currentPolicy->allocate()`."

2. **Q: Show me multilevel inheritance in your code.**  
   *A:* "In `Laboratory.hpp` line 20: `class Laboratory : public AcademicBuilding`, where `AcademicBuilding` in turn inherits from `Building`. `Laboratory` inherits classrooms from `AcademicBuilding` and ID/demands from `Building`."

3. **Q: Why did you declare virtual destructors in `Building` and `EnergySource`?**  
   *A:* "Because `Campus` deletes derived objects (`Laboratory`, `Hostel`) polymorphically through a base `Building*` inside `std::unique_ptr<Building>`, and a virtual destructor guarantees the derived destructor executes first, preventing memory leaks."

4. **Q: What design pattern did you use for the allocation policies?**  
   *A:* "We used the Strategy Design Pattern. `AllocationEngine` aggregates an abstract `AllocationPolicy` pointer, allowing `PriorityPolicy` and `FairSharePolicy` algorithms to be swapped at runtime without changing the engine code."

5. **Q: How does `Campus` manage memory without memory leaks?**  
   *A:* "By using `std::vector<std::unique_ptr<Building>>`. Modern C++ RAII ensures that when `Campus` goes out of scope, all dynamically allocated buildings are destroyed automatically without calling manual `delete`."

6. **Q: What is the difference between your Priority policy and Fair-Share policy?**  
   *A:* "Priority Policy satisfies critical and high-priority buildings completely before lower-priority buildings get any discretionary power, whereas Fair-Share satisfies essential baselines first and then distributes remaining discretionary power proportionally to prevent lower-priority buildings from starving."

7. **Q: Where did you use function and class templates?**  
   *A:* "In `include/smartcampus/utils/GenericUtils.hpp`, where we implemented the generic function template `clampValue<T>` and the generic class template `MetricAggregator<T>` to compute campus-wide statistical averages."

8. **Q: Why did you delete the copy constructor on `Building`?**  
   *A:* "To prevent object slicing, which occurs if a derived class object like `Laboratory` is accidentally copied into a base `Building` value, slicing off its derived member variables."
