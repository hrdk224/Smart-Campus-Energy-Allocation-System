# Smart Campus Energy Allocation System

> **A Small, Beginner-Friendly C++ Course Project for 4 Students**  
> Covers all Units of the B.Tech Object-Oriented Programming (OOP) Syllabus without complex design patterns.

---

## 1. Project Overview & Allocation Logic

The campus has three buildings with different energy requirements and priority levels:
1. **Campus Clinic (ID: 1):** Priority **P1** (Highest — Critical healthcare, emergency beds, ambulance bay). Requested: **40.0 kWh**.
2. **Classroom Block (ID: 2):** Priority **P2** (Medium — Academic lecture halls, smart boards). Requested: **50.0 kWh**.
3. **Hostel (ID: 3):** Priority **P3** (Lowest — Student residential rooms). Requested: **30.0 kWh**.

**Total Campus Demand = 120.0 kWh.**

### The Allocation Rule (Easy to Say in Viva):
> **“Sort by priority, then give each building as much as possible until the supply runs out. For equal priorities, preserve original order.”**

- If **Supply = 90.0 kWh** (Shortage: Solar 40 + Grid 50):
  - Clinic (P1) gets **40.0 kWh** (Fully Met).
  - Classroom Block (P2) gets remaining **50.0 kWh** (Fully Met).
  - Hostel (P3) gets **0.0 kWh** (Deferred due to 30.0 kWh shortage).
- If **Supply = 150.0 kWh** (Surplus: Solar 80 + Grid 70):
  - All three buildings get 100% of their requested energy (120.0 kWh allocated, 0 remaining).

---

## 2. Repository File Structure

This repository deliberately uses only a few flat, easily understood files:

```
SmartCampusEnergyAllocation/
├── campus.h          # Shared class declarations, structs, and function prototypes
├── buildings.cpp     # Building classes, constructors, operators, and viva demos (Student 1)
├── allocation.cpp    # Priority allocation algorithm and table display (Student 2)
├── files.cpp         # Text and binary file operations, seekg/seekp (Student 3)
├── main.cpp          # Interactive menu, sample data, exception handling (Student 4)
├── README.md         # Compilation, syllabus checklist, and viva walkthrough
└── .gitignore        # Ignores compiled binary and generated data files
```

---

## 3. How to Compile & Run

### Direct `g++` Compilation Command
Compile directly in your terminal using standard `g++` (C++17):

```bash
g++ -std=c++17 -Wall -Wextra main.cpp buildings.cpp allocation.cpp files.cpp -o campus_sim
```

### Run Interactive Console Menu
```bash
./campus_sim
```

### Run Automated 2-Minute Demo Mode
```bash
./campus_sim --demo
```

*(Note: Data files `allocation_report.txt` and `allocations.dat` are created only when the program runs and are excluded from git).*

---

## 4. Key Syllabus Theoretical Concepts

### Non-Modular vs. Modular vs. OOP Approach (Unit 1)
- **Non-Modular (Monolithic):** Everything is written inside a single huge `main()` function with global variables. Hard to debug, impossible to maintain.
- **Modular (Procedural):** Code is split into separate functions and translation units (`.cpp` files). Data and functions are separate; data is still vulnerable to unintentional modification.
- **Object-Oriented Programming (OOP):** Binds data and methods into **classes** with access control (`private`, `public`). Encapsulates state, models real-world entities through inheritance, and enables runtime flexibility through polymorphism.

### Diamond Inheritance & Virtual Base Class (Unit 3)
```
             Building (virtual base class)
              /      \
  MedicalFacility   EmergencyUnit
              \      /
            CampusClinic
```
- **The Problem:** In multiple inheritance, if `CampusClinic` inherits from both `MedicalFacility` and `EmergencyUnit`, and both inherit from `Building`, `CampusClinic` would receive **two separate copies** of `Building` (duplicate `id`, `name`, `priority`), causing compiler ambiguity.
- **The Solution:** By declaring `virtual public Building` in both intermediate classes, C++ guarantees that only **one shared copy** of `Building` exists inside `CampusClinic`.

### C++ Stream Hierarchy & Formatted Output (Unit 4)
- **Base Class:** `ios_base` and `ios` manage stream state and formatting flags.
- **Input/Output Streams:** `istream` (for reading, e.g. `cin`) and `ostream` (for writing, e.g. `cout`).
- **File Streams:** `ifstream` (inherits from `istream` for reading files), `ofstream` (inherits from `ostream` for writing files), and `fstream` (inherits from `iostream` for simultaneous read/write).
- **String Streams:** `stringstream` (inherits from `iostream` to format strings in memory).
- **Formatting:** `setw(width)` sets field column widths, `fixed` disables scientific notation, and `setprecision(1)` fixes decimals to 1 decimal place.

### Why Fixed-Size Binary Records for Random Access (`seekg`/`seekp`)? (Unit 4)
- Text files contain lines of varying lengths, so jumping directly to line $N$ requires reading all preceding lines sequentially ($O(N)$).
- A fixed-size binary struct `BuildingRecord` occupies an exact, constant number of bytes (`sizeof(BuildingRecord) = 64` bytes).
- To read or update record $N$, the program jumps directly in $O(1)$ constant time using:
  $$\text{Byte Offset} = N \times \text{sizeof}(\text{BuildingRecord})$$
  - `file.seekg(offset, ios::beg)` positions the get-pointer for instant reading.
  - `file.seekp(offset, ios::beg)` positions the put-pointer for instant in-place updating.

### Why Shortages Are NOT Exceptions (Unit 5)
- An energy shortage (supply < demand) is a **normal, expected operational condition** of an energy allocation system, not an exceptional system failure. It is handled by the priority allocation logic.
- **Exceptions** are strictly reserved for genuine errors: invalid user inputs (negative values) and file I/O failures (unable to open a file).

---

## 5. Comprehensive Syllabus Checklist Table

| Unit | Syllabus Concept | File & Function / Class | One-Sentence Viva Explanation |
| :---: | :--- | :--- | :--- |
| **1** | Classes, objects, access specifiers (`private`, `protected`, `public`) | [`campus.h`](campus.h), `Building` | `Building` hides private `id` and `name`, exposes protected `priority` to derived classes, and provides public methods. |
| **1** | Pointers vs. Reference variables | [`buildings.cpp`](buildings.cpp), `demonstratePointersAndRefs()` | A pointer holds a memory address and can be reseated or null; a reference is an immutable alias to an existing object. |
| **1** | Static data member & static function | [`campus.h`](campus.h), `Building::totalBuildingsCreated`, `getTotalBuildings()` | Tracks total building objects instantiated across the entire program without needing a specific object instance. |
| **1** | Inline function | [`campus.h`](campus.h), `Building::getRemainingKwh()` | Inline function hints compiler to substitute body directly at call site to eliminate function call overhead. |
| **1** | Function overloading | [`allocation.cpp`](allocation.cpp), `calculateTotalSupply()` | Overloaded for `double` parameters and `EnergyAmount` objects, resolved at compile-time by parameter types. |
| **2** | Dynamic memory (`new` and `delete`) | [`buildings.cpp`](buildings.cpp), `demonstrateNewDelete()` | Allocates an object on the heap with `new`, destroys it with `delete`, and sets pointer to `nullptr` to avoid dangling pointers. |
| **2** | Default, Parameterized, Copy Constructor & Destructor | [`campus.h`](campus.h), [`buildings.cpp`](buildings.cpp), `EnergyAmount` | Shows complete lifecycle: default sets 0, parameterized initializes kwh, copy copies value, destructor cleans up. |
| **2** | `this` pointer & Initializer list | [`buildings.cpp`](buildings.cpp), `Building::Building(...)` | Initializer list directly initializes member fields before constructor body; `this` pointer disambiguates object scope. |
| **2** | Friend function | [`campus.h`](campus.h), `inspectEmergencyReserve(const CampusClinic&)` | Non-member function granted explicit access to private clinic member `secretBackupGeneratorKw`. |
| **2** | Friend class | [`campus.h`](campus.h), `EnergyAuditor::auditBuilding(const Building&)` | Dedicated auditing class granted access to inspect private and protected members of `Building`. |
| **2** | Operator Overloading (Unary & Binary) | [`campus.h`](campus.h), [`buildings.cpp`](buildings.cpp), `EnergyAmount` | Unary `!` checks if energy is zero; binary `+` adds two energy quantities; `<<` formats stream output. |
| **2** | Array of objects & Dynamic pointer | [`main.cpp`](main.cpp), `Building* campus[3]` | Array of base class pointers storing addresses of dynamically allocated or concrete derived building objects. |
| **3** | Single Inheritance | [`campus.h`](campus.h), `ClassroomBlock : public Building` | `ClassroomBlock` inherits directly from `Building`, adding classroom counts. |
| **3** | Multilevel Inheritance | [`campus.h`](campus.h), `Building` $\to$ `ResidentialBuilding` $\to$ `Hostel` | `Hostel` inherits from `ResidentialBuilding`, which in turn inherits from `Building`. |
| **3** | Hierarchical Inheritance | [`campus.h`](campus.h), `Building` inherited by multiple classes | `ClassroomBlock`, `ResidentialBuilding`, and `MedicalFacility` all branch from base `Building`. |
| **3** | Multiple Inheritance | [`campus.h`](campus.h), `CampusClinic : MedicalFacility, EmergencyUnit` | `CampusClinic` inherits features from both `MedicalFacility` (doctors) and `EmergencyUnit` (ambulances). |
| **3** | Hybrid / Diamond Inheritance with `virtual` base | [`campus.h`](campus.h), `MedicalFacility`, `EmergencyUnit` $\to$ `CampusClinic` | Declaring `virtual public Building` prevents duplicate copies of `Building` inside `CampusClinic`. |
| **3** | Derived constructor passing args to base constructor | [`buildings.cpp`](buildings.cpp), `CampusClinic::CampusClinic(...)` | Derived constructor explicitly passes ID, name, and demands to virtual base `Building` and intermediate parents. |
| **3** | Constructor/Destructor call order | [`buildings.cpp`](buildings.cpp), `demonstrateConstructorDestructorOrder()` | Constructors run from base to derived; destructors execute in exact reverse order when object leaves scope. |
| **3** | Pure virtual function & Abstract class | [`campus.h`](campus.h), `virtual void showDetails() const = 0;` | `Building` cannot be instantiated directly, forcing derived classes to provide their own implementation. |
| **3** | Runtime Polymorphism (Late Binding) | [`allocation.cpp`](allocation.cpp), `Building* b` calling overridden methods | Base pointer calls `b->showDetails()` dynamically resolved at runtime through the virtual method table (vtable). |
| **3** | Virtual Destructor | [`campus.h`](campus.h), `virtual ~Building();` | Guarantees that deleting an object through a `Building*` pointer safely invokes derived destructors first. |
| **4** | Stream hierarchy (`ifstream`, `ofstream`, `fstream`, `stringstream`) | [`files.cpp`](files.cpp) | Demonstrates writing with `ofstream`, reading with `ifstream`, read/write with `fstream`, and formatting with `stringstream`. |
| **4** | Random file access (`seekg` and `seekp`) | [`files.cpp`](files.cpp), `readBinaryRecordAt()`, `updateBinaryRecordAt()` | `seekg` moves read pointer and `seekp` moves write pointer to `index * sizeof(Record)` for $O(1)$ access. |
| **5** | Exception handling (`try`, `throw`, `catch`) | [`main.cpp`](main.cpp), [`allocation.cpp`](allocation.cpp) | Rejects negative inputs and file failures using `throw invalid_argument` and catches in `main()`. |
| **5** | `catch (...)` and re-throw (`throw;`) | [`main.cpp`](main.cpp), `demonstrateCatchAllAndRethrow()` | Catches any unknown exception using `catch (...)`, logs the incident, and re-throws it to the caller with `throw;`. |

---

## 6. Four-Student Work Division

| Student | Assigned File | Git Branch | Responsibilities & Functions | Peer Reviewer |
| :--- | :--- | :--- | :--- | :--- |
| **Student 1** | `buildings.cpp` & `campus.h` | `feature/buildings` | • Implement `EnergyAmount` (constructors, operators `!`, `+`, `-`, `<<`).<br>• Implement `Building`, `ClassroomBlock`, `Hostel`, `CampusClinic`.<br>• Implement Diamond inheritance and static building counter.<br>• Implement friend function and friend class. | Student 2 |
| **Student 2** | `allocation.cpp` | `feature/allocation` | • Overload `calculateTotalSupply()` for `double` and `EnergyAmount`.<br>• Implement `allocateCampusEnergy()` (sort by priority, allocate until supply runs out).<br>• Input validation (reject negative supply).<br>• Implement `displayAllocationTable()`. | Student 3 |
| **Student 3** | `files.cpp` | `feature/files` | • Implement `saveTextReport()` and `loadTextReport()`.<br>• Implement binary records with `BuildingRecord`.<br>• Implement `readBinaryRecordAt()` using `seekg()`.<br>• Implement `updateBinaryRecordAt()` using `seekp()`.<br>• Implement `demonstrateStringStream()`. | Student 4 |
| **Student 4** | `main.cpp` & `README.md` | `feature/main-integration` | • Implement interactive console menu.<br>• Set up 3 campus buildings array `Building* campus[3]`.<br>• Implement `demonstrateCatchAllAndRethrow()`.<br>• Add automated `--demo` flag.<br>• Verify build with `g++` and run all 3 test scenarios. | Student 1 |

---

## 7. Results of the Three Test Scenarios

### Scenario 1: Shortage (Supply 90 kWh < Demand 120 kWh)
- **Inputs:** Solar = 40.0 kWh, Grid = 50.0 kWh $\to$ **Total Supply = 90.0 kWh**.
- **Demand:** Clinic (40) + Classrooms (50) + Hostel (30) = **120.0 kWh**.
- **Output:**
  - **Campus Clinic (P1):** Requested 40.0 kWh $\to$ **Allocated 40.0 kWh** (FULLY MET, 0 Remaining).
  - **Classroom Block (P2):** Requested 50.0 kWh $\to$ **Allocated 50.0 kWh** (FULLY MET, 0 Remaining).
  - **Mandela Hostel (P3):** Requested 30.0 kWh $\to$ **Allocated 0.0 kWh** (DEFERRED, 30.0 Remaining).
- **Result:** Priority rule clearly demonstrated. Critical clinic and classrooms are satisfied; hostel load deferred.

### Scenario 2: Surplus (Supply 150 kWh > Demand 120 kWh)
- **Inputs:** Solar = 80.0 kWh, Grid = 70.0 kWh $\to$ **Total Supply = 150.0 kWh**.
- **Demand:** 120.0 kWh.
- **Output:**
  - **Campus Clinic (P1):** Allocated **40.0 kWh** (FULLY MET).
  - **Classroom Block (P2):** Allocated **50.0 kWh** (FULLY MET).
  - **Mandela Hostel (P3):** Allocated **30.0 kWh** (FULLY MET).
- **Result:** 100% of campus building demands satisfied.

### Scenario 3: Negative Input Exception Handling
- **Input:** Solar = -20.0 kWh.
- **Output:**
  ```text
  [INPUT ERROR] Solar energy cannot be negative: -20.000000
  ```
- **Result:** Caught by `catch (const invalid_argument& e)` in `main()`. Program rejects bad input gracefully without crashing.

---

## 8. Two-Minute Presentation & Viva Walkthrough Script

Use this exact 2-minute script when presenting to your examiner:

- **Minute 0:00 - 0:30 (Student 1):**  
  > *"Respected examiner, our project models a campus with three facilities: a Clinic, a Classroom Block, and a Hostel. I implemented `campus.h` and `buildings.cpp`. We demonstrate all five forms of inheritance: Single in `ClassroomBlock`, Multilevel in `Hostel`, and a Diamond hybrid hierarchy where `CampusClinic` inherits from `MedicalFacility` and `EmergencyUnit` with `virtual public Building` to eliminate ambiguity. We also implemented static counters, virtual destructors, and operator overloading on our `EnergyAmount` class."*

- **Minute 0:30 - 1:00 (Student 2):**  
  > *"I developed `allocation.cpp`. Our allocation algorithm is straightforward: 'Sort by priority, then give each building as much as possible until the supply runs out.' In our shortage scenario where supply is 90 kWh and demand is 120 kWh, the Clinic with priority P1 receives its full 40 kWh, Classrooms with P2 receive 50 kWh, and the Hostel with P3 receives 0 kWh. I also demonstrated compile-time polymorphism through function overloading."*

- **Minute 1:00 - 1:30 (Student 3):**  
  > *"I developed `files.cpp`. We demonstrate the full C++ stream hierarchy using `ifstream`, `ofstream`, `fstream`, and `stringstream`. For binary operations, we used a fixed-size `BuildingRecord` struct. Because the record size is fixed at 64 bytes, we achieve $O(1)$ random access: `file.seekg(offset)` jumps directly to any record to read it, and `file.seekp(offset)` updates it in-place."*

- **Minute 1:30 - 2:00 (Student 4):**  
  > *"I integrated the system in `main.cpp`. We use base class pointers `Building* campus[3]` to call overridden methods at runtime, demonstrating late binding. We handle errors defensively using `try`, `throw`, and `catch`, rejecting negative inputs and missing files, and demonstrating `catch(...)` with `throw;` for re-throwing. Let us now run `./campus_sim --demo` to see all three scenarios live."*
