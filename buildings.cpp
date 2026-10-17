#include "campus.h"
#include <iostream>
#include <iomanip>

using namespace std;

// ============================================================================
// EnergyAmount Implementation (Unit 2)
// ============================================================================

// Default constructor
EnergyAmount::EnergyAmount() : kwh(0.0) {
    // Member initializer list sets kwh to 0.0
}

// Parameterized constructor using 'this' pointer
EnergyAmount::EnergyAmount(double val) {
    if (val < 0.0) {
        throw invalid_argument("Negative energy value rejected: " + to_string(val));
    }
    this->kwh = val; // 'this' pointer disambiguates and assigns
}

// Copy constructor
EnergyAmount::EnergyAmount(const EnergyAmount& other) {
    this->kwh = other.kwh;
}

// Destructor
EnergyAmount::~EnergyAmount() {
    // Destructor cleanly executed when EnergyAmount goes out of scope
}

double EnergyAmount::getKwh() const {
    return this->kwh;
}

void EnergyAmount::setKwh(double val) {
    if (val < 0.0) {
        throw invalid_argument("Negative energy value rejected: " + to_string(val));
    }
    this->kwh = val;
}

// Unary operator overload: returns true if energy is zero
bool EnergyAmount::operator!() const {
    return this->kwh <= 0.0001;
}

// Binary operator +
EnergyAmount EnergyAmount::operator+(const EnergyAmount& other) const {
    return EnergyAmount(this->kwh + other.kwh);
}

// Binary operator -
EnergyAmount EnergyAmount::operator-(const EnergyAmount& other) const {
    double diff = this->kwh - other.kwh;
    return EnergyAmount((diff > 0.0) ? diff : 0.0);
}

// Binary operator <
bool EnergyAmount::operator<(const EnergyAmount& other) const {
    return this->kwh < other.kwh;
}

// Binary operator ==
bool EnergyAmount::operator==(const EnergyAmount& other) const {
    return this->kwh == other.kwh;
}

// Friend stream insertion operator
ostream& operator<<(ostream& os, const EnergyAmount& e) {
    os << fixed << setprecision(1) << e.kwh << " kWh";
    return os;
}

// ============================================================================
// Building Base Class Implementation (Unit 1 & Unit 3)
// ============================================================================

// Initialize static data member (Unit 1)
int Building::totalBuildingsCreated = 0;

// Parameterized constructor using member initializer list and 'this' pointer
Building::Building(int id, string name, int priority, double reqKwh)
    : id(id), name(name), priority(priority), requested(reqKwh), allocated(0.0) {
    this->totalBuildingsCreated++; // Increment static counter
}

// Virtual destructor (Unit 3: Polymorphic cleanup)
Building::~Building() {
    this->totalBuildingsCreated--; // Decrement static counter
}

// Static member function (Unit 1)
int Building::getTotalBuildings() {
    return totalBuildingsCreated;
}

void Building::setAllocatedKwh(double kwh) {
    if (kwh < 0.0) {
        throw invalid_argument("Allocated energy cannot be negative!");
    }
    this->allocated.setKwh(kwh);
}

void Building::setRequestedKwh(double kwh) {
    if (kwh < 0.0) {
        throw invalid_argument("Requested energy cannot be negative!");
    }
    this->requested.setKwh(kwh);
}

// ============================================================================
// Single Inheritance: ClassroomBlock (Unit 3)
// ============================================================================
ClassroomBlock::ClassroomBlock(int id, string name, int priority, double reqKwh, int rooms)
    : Building(id, name, priority, reqKwh), classroomCount(rooms) {}

ClassroomBlock::~ClassroomBlock() {}

void ClassroomBlock::showDetails() const {
    cout << "[ClassroomBlock] ID: " << getId() << " | Name: " << getName()
         << " | Priority: P" << getPriority()
         << " | Classrooms: " << classroomCount
         << " | Requested: " << getRequestedKwh() << " kWh"
         << " | Allocated: " << getAllocatedKwh() << " kWh"
         << " | Remaining: " << getRemainingKwh() << " kWh\n";
}

string ClassroomBlock::getCategory() const {
    return "Academic";
}

// ============================================================================
// Multilevel Inheritance: ResidentialBuilding -> Hostel (Unit 3)
// ============================================================================
ResidentialBuilding::ResidentialBuilding(int id, string name, int priority, double reqKwh, int capacity)
    : Building(id, name, priority, reqKwh), residentCapacity(capacity) {}

ResidentialBuilding::~ResidentialBuilding() {}

void ResidentialBuilding::showDetails() const {
    cout << "[Residential] ID: " << getId() << " | Name: " << getName()
         << " | Capacity: " << residentCapacity << " residents\n";
}

string ResidentialBuilding::getCategory() const {
    return "Residential Base";
}

Hostel::Hostel(int id, string name, int priority, double reqKwh, int capacity, int rooms)
    : ResidentialBuilding(id, name, priority, reqKwh, capacity), roomCount(rooms) {}

Hostel::~Hostel() {}

void Hostel::showDetails() const {
    cout << "[Hostel] ID: " << getId() << " | Name: " << getName()
         << " | Priority: P" << getPriority()
         << " | Residents: " << residentCapacity << " in " << roomCount << " rooms"
         << " | Requested: " << getRequestedKwh() << " kWh"
         << " | Allocated: " << getAllocatedKwh() << " kWh"
         << " | Remaining: " << getRemainingKwh() << " kWh\n";
}

string Hostel::getCategory() const {
    return "Student Housing";
}

// ============================================================================
// Hierarchical, Multiple & Hybrid / Diamond Inheritance: CampusClinic (Unit 3)
// ============================================================================
MedicalFacility::MedicalFacility(int id, string name, int priority, double reqKwh, int doctors)
    : Building(id, name, priority, reqKwh), doctorCount(doctors) {}

MedicalFacility::~MedicalFacility() {}

void MedicalFacility::showDetails() const {
    cout << "[MedicalFacility] Doctors: " << doctorCount << "\n";
}

string MedicalFacility::getCategory() const {
    return "Medical";
}

EmergencyUnit::EmergencyUnit(int id, string name, int priority, double reqKwh, bool ambulance)
    : Building(id, name, priority, reqKwh), hasAmbulanceBay(ambulance) {}

EmergencyUnit::~EmergencyUnit() {}

void EmergencyUnit::showDetails() const {
    cout << "[EmergencyUnit] Ambulance Bay: " << (hasAmbulanceBay ? "Yes" : "No") << "\n";
}

string EmergencyUnit::getCategory() const {
    return "Emergency";
}

// CampusClinic constructor: Notice how virtual base class 'Building' is explicitly initialized
CampusClinic::CampusClinic(int id, string name, int priority, double reqKwh, int doctors, bool ambulance, int beds)
    : Building(id, name, priority, reqKwh),
      MedicalFacility(id, name, priority, reqKwh, doctors),
      EmergencyUnit(id, name, priority, reqKwh, ambulance),
      emergencyBeds(beds),
      secretBackupGeneratorKw(25.0) {}

CampusClinic::~CampusClinic() {}

void CampusClinic::showDetails() const {
    cout << "[CampusClinic] ID: " << getId() << " | Name: " << getName()
         << " | Priority: P" << getPriority() << " (CRITICAL HEALTHCARE)"
         << " | Doctors: " << doctorCount
         << " | Ambulance: " << (hasAmbulanceBay ? "Ready" : "No")
         << " | Beds: " << emergencyBeds
         << " | Requested: " << getRequestedKwh() << " kWh"
         << " | Allocated: " << getAllocatedKwh() << " kWh"
         << " | Remaining: " << getRemainingKwh() << " kWh\n";
}

string CampusClinic::getCategory() const {
    return "Healthcare & Emergency";
}

// ============================================================================
// Friend Function & Friend Class Implementations (Unit 2)
// ============================================================================

// Friend Function: can access private member 'secretBackupGeneratorKw' of CampusClinic
void inspectEmergencyReserve(const CampusClinic& clinic) {
    cout << "  [Friend Function Inspection] Accessing private clinic generator: "
         << clinic.secretBackupGeneratorKw << " kW backup available for emergencies.\n";
}

// Friend Class: EnergyAuditor can access private 'id' and 'name' of Building
void EnergyAuditor::auditBuilding(const Building& b) {
    cout << "  [Friend Class EnergyAuditor] Auditing private record: Building #"
         << b.id << " (" << b.name << ") Priority=P" << b.priority
         << " | Current Allocation=" << b.getAllocatedKwh() << " kWh\n";
}

// ============================================================================
// Small OOP Demonstration Functions for Viva (Units 1-3)
// ============================================================================

void demonstratePointersAndRefs() {
    cout << "\n--- DEMO: POINTERS VS REFERENCE VARIABLES (UNIT 1) ---\n";
    ClassroomBlock room(101, "Lecture Hall A", 2, 50.0, 4);

    // Pointer variable: stores address, can be reseated or null
    Building* ptr = &room;

    // Reference variable: alias to original object, cannot be null
    Building& ref = room;

    cout << "Original Building Name: " << room.getName() << "\n";
    cout << "Accessed via Pointer:   " << ptr->getName() << " (address: " << ptr << ")\n";
    cout << "Accessed via Reference: " << ref.getName() << " (alias directly)\n";
}

void demonstrateConstructorDestructorOrder() {
    cout << "\n--- DEMO: CONSTRUCTOR & DESTRUCTOR CALL ORDER (UNIT 3) ---\n";
    cout << "Creating CampusClinic inside a local scope block:\n";
    {
        // Demonstrates constructor execution order:
        // 1. Virtual base 'Building'
        // 2. Base 'MedicalFacility'
        // 3. Base 'EmergencyUnit'
        // 4. Derived 'CampusClinic'
        cout << "  -> Entering block: instantiating CampusClinic...\n";
        CampusClinic tempClinic(999, "Temporary Health Booth", 1, 15.0, 1, false, 2);
        tempClinic.showDetails();
        cout << "  -> Exiting block: scope ending, destructors will run in REVERSE order...\n";
    }
    cout << "Scope exited successfully. All destructors executed cleanly.\n";
}

void demonstrateNewDelete() {
    cout << "\n--- DEMO: DYNAMIC MEMORY ALLOCATION (NEW AND DELETE) (UNIT 2) ---\n";
    cout << "Active buildings before allocation: " << Building::getTotalBuildings() << "\n";

    // Allocating dynamic object on heap
    Building* dynamicBuilding = new ClassroomBlock(202, "New Computer Lab", 2, 45.0, 6);
    cout << "Dynamically allocated: " << dynamicBuilding->getName() << " at address " << dynamicBuilding << "\n";
    cout << "Active buildings after new: " << Building::getTotalBuildings() << "\n";

    // Deleting object safely through polymorphic base pointer
    delete dynamicBuilding;
    dynamicBuilding = nullptr; // Avoid dangling pointer!

    cout << "Deleted object cleanly and reset pointer to nullptr.\n";
    cout << "Active buildings after delete: " << Building::getTotalBuildings() << "\n";
}

void demonstrateDiamondInheritance() {
    cout << "\n--- DEMO: DIAMOND INHERITANCE RESOLUTION WITH VIRTUAL BASE (UNIT 3) ---\n";
    cout << "Structure:\n"
         << "           Building (virtual base)\n"
         << "            /    \\\n"
         << "MedicalFacility  EmergencyUnit\n"
         << "            \\    /\n"
         << "         CampusClinic\n";

    CampusClinic clinic(1, "Main Campus Clinic", 1, 40.0, 3, true, 8);

    // Call through base class pointers showing Runtime Polymorphism (Late Binding)
    Building* bPtr = &clinic;
    cout << "Calling overridden showDetails() through Building* base pointer:\n  ";
    bPtr->showDetails();

    // Friend function inspection
    inspectEmergencyReserve(clinic);

    // Friend class audit
    EnergyAuditor::auditBuilding(clinic);
}

void demonstrateOperatorOverloading() {
    cout << "\n--- DEMO: OPERATOR OVERLOADING ON EnergyAmount (UNIT 2) ---\n";
    EnergyAmount e1(40.0);
    EnergyAmount e2(50.0);
    EnergyAmount zero(0.0);

    cout << "e1 = " << e1 << " | e2 = " << e2 << " | zero = " << zero << "\n";

    // Binary +
    EnergyAmount sum = e1 + e2;
    cout << "Binary operator+: e1 + e2 = " << sum << "\n";

    // Binary -
    EnergyAmount diff = e2 - e1;
    cout << "Binary operator-: e2 - e1 = " << diff << "\n";

    // Binary <
    cout << "Binary operator<: (e1 < e2) = " << ((e1 < e2) ? "true" : "false") << "\n";

    // Unary !
    cout << "Unary operator!: (!e1) = " << ((!e1) ? "true (no energy)" : "false (has energy)") << "\n";
    cout << "Unary operator!: (!zero) = " << ((!zero) ? "true (no energy)" : "false (has energy)") << "\n";
}
