#ifndef CAMPUS_H
#define CAMPUS_H

#include <iostream>
#include <string>
#include <stdexcept>

// ============================================================================
// UNIT 2: EnergyAmount Class
// Demonstrates:
// - Default, parameterized, and copy constructors; destructor
// - 'this' pointer and constructor initializer list
// - Operator overloading: Unary (!) and Binary (+, -, ==, <, <<)
// ============================================================================
class EnergyAmount {
private:
    double kwh;

public:
    // Constructors & Destructor
    EnergyAmount();                                // Default constructor
    EnergyAmount(double val);                      // Parameterized constructor
    EnergyAmount(const EnergyAmount& other);       // Copy constructor
    ~EnergyAmount();                               // Destructor

    // Getter and Setter
    double getKwh() const;
    void setKwh(double val);

    // Operator Overloading
    // Unary operator: returns true if energy is zero or negative
    bool operator!() const;

    // Binary operators
    EnergyAmount operator+(const EnergyAmount& other) const;
    EnergyAmount operator-(const EnergyAmount& other) const;
    bool operator<(const EnergyAmount& other) const;
    bool operator==(const EnergyAmount& other) const;

    // Stream insertion operator (Friend function)
    friend std::ostream& operator<<(std::ostream& os, const EnergyAmount& e);
};

// ============================================================================
// UNIT 4: Fixed-Size Record for Binary File Operations & Random Access (seekg/seekp)
// Why fixed-size? Fixed byte count allows direct O(1) jumping: offset = index * sizeof(Record)
// ============================================================================
struct BuildingRecord {
    int id;
    char name[32];
    int priority;
    double requestedKwh;
    double allocatedKwh;
    double remainingKwh;
};

// Forward declaration for friend class and friend function
class Building;
class CampusClinic;

// ============================================================================
// UNIT 2: Friend Class
// EnergyAuditor can inspect private/protected internals of Building
// ============================================================================
class EnergyAuditor {
public:
    static void auditBuilding(const Building& b);
};

// ============================================================================
// UNIT 1 & UNIT 3: Abstract Base Class 'Building'
// Demonstrates:
// - Abstraction & pure virtual functions (showDetails, getCategory)
// - Virtual destructor for safe polymorphic deletion
// - Static data member and static function
// - Access specifiers: private, protected, public
// - Virtual base class to solve the diamond problem
// ============================================================================
class Building {
private:
    int id;
    std::string name;

    // Static data member: tracks total building objects created
    static int totalBuildingsCreated;

protected:
    int priority;              // Lower number = higher priority (1 is highest)
    EnergyAmount requested;    // Requested energy in kWh
    EnergyAmount allocated;    // Allocated energy in kWh

public:
    // Parameterized constructor using initializer list and 'this' pointer
    Building(int id = 0, std::string name = "Generic", int priority = 3, double reqKwh = 0.0);
    virtual ~Building();       // Virtual destructor for polymorphic base class

    // Static member function
    static int getTotalBuildings();

    // Inline member functions (Unit 1)
    inline int getId() const { return id; }
    inline std::string getName() const { return name; }
    inline int getPriority() const { return priority; }
    inline double getRequestedKwh() const { return requested.getKwh(); }
    inline double getAllocatedKwh() const { return allocated.getKwh(); }
    inline double getRemainingKwh() const {
        double rem = requested.getKwh() - allocated.getKwh();
        return (rem > 0.0) ? rem : 0.0;
    }
    inline bool isFullySatisfied() const {
        return allocated.getKwh() >= requested.getKwh();
    }

    // Setters
    void setAllocatedKwh(double kwh);
    void setRequestedKwh(double kwh);

    // Pure virtual functions: makes Building an Abstract Base Class
    virtual void showDetails() const = 0;
    virtual std::string getCategory() const = 0;

    // Friend class declaration
    friend class EnergyAuditor;
};

// ============================================================================
// UNIT 3: Single Inheritance
// ClassroomBlock is-a Building
// ============================================================================
class ClassroomBlock : public Building {
private:
    int classroomCount;

public:
    ClassroomBlock(int id, std::string name, int priority, double reqKwh, int rooms);
    ~ClassroomBlock() override;

    void showDetails() const override;
    std::string getCategory() const override;
};

// ============================================================================
// UNIT 3: Multilevel Inheritance
// Building -> ResidentialBuilding -> Hostel
// ============================================================================
class ResidentialBuilding : public Building {
protected:
    int residentCapacity;

public:
    ResidentialBuilding(int id, std::string name, int priority, double reqKwh, int capacity);
    ~ResidentialBuilding() override;

    void showDetails() const override;
    std::string getCategory() const override;
};

class Hostel : public ResidentialBuilding {
private:
    int roomCount;

public:
    Hostel(int id, std::string name, int priority, double reqKwh, int capacity, int rooms);
    ~Hostel() override;

    void showDetails() const override;
    std::string getCategory() const override;
};

// ============================================================================
// UNIT 3: Hierarchical, Multiple & Hybrid / Diamond Inheritance with Virtual Base Class
//
//                 Building (virtual base class)
//                  /      \
//      MedicalFacility   EmergencyUnit  (Hierarchical from Building)
//                  \      /
//                CampusClinic           (Multiple from Medical & Emergency; Hybrid Diamond)
// ============================================================================
class MedicalFacility : virtual public Building {
protected:
    int doctorCount;

public:
    MedicalFacility(int id = 0, std::string name = "Medical", int priority = 1, double reqKwh = 0.0, int doctors = 2);
    ~MedicalFacility() override;

    void showDetails() const override;
    std::string getCategory() const override;
};

class EmergencyUnit : virtual public Building {
protected:
    bool hasAmbulanceBay;

public:
    EmergencyUnit(int id = 0, std::string name = "Emergency", int priority = 1, double reqKwh = 0.0, bool ambulance = true);
    ~EmergencyUnit() override;

    void showDetails() const override;
    std::string getCategory() const override;
};

// CampusClinic inherits from both MedicalFacility and EmergencyUnit (Multiple + Diamond)
class CampusClinic : public MedicalFacility, public EmergencyUnit {
private:
    int emergencyBeds;
    double secretBackupGeneratorKw; // Inspected by friend function

public:
    CampusClinic(int id, std::string name, int priority, double reqKwh, int doctors, bool ambulance, int beds);
    ~CampusClinic() override;

    void showDetails() const override;
    std::string getCategory() const override;

    // Friend function: grants external inspection function access to private secretBackupGeneratorKw
    friend void inspectEmergencyReserve(const CampusClinic& clinic);
};

// Friend function declaration
void inspectEmergencyReserve(const CampusClinic& clinic);

// ============================================================================
// Shared Allocation Functions (Implemented in allocation.cpp)
// ============================================================================
// Function overloading (Unit 1)
double calculateTotalSupply(double solarKwh, double gridKwh);
EnergyAmount calculateTotalSupply(const EnergyAmount& solar, const EnergyAmount& grid);

// Core priority allocation algorithm
void allocateCampusEnergy(Building* buildings[], int count, double totalSupplyKwh);

// Display current status table
void displayAllocationTable(Building* buildings[], int count, double totalSupplyKwh);

// ============================================================================
// Shared File Functions (Implemented in files.cpp)
// ============================================================================
// Text file I/O
void saveTextReport(const std::string& filename, Building* buildings[], int count, double solar, double grid);
void loadTextReport(const std::string& filename);

// Binary file I/O and Random Access (seekg / seekp)
void saveBinaryRecords(const std::string& filename, Building* buildings[], int count);
void readBinaryRecordAt(const std::string& filename, int recordIndex);
void updateBinaryRecordAt(const std::string& filename, int recordIndex, double newAllocatedKwh);

// String stream demonstration
void demonstrateStringStream(const Building& b);

// ============================================================================
// Small OOP Demonstrations (Implemented in buildings.cpp)
// ============================================================================
void demonstratePointersAndRefs();
void demonstrateConstructorDestructorOrder();
void demonstrateNewDelete();
void demonstrateDiamondInheritance();
void demonstrateOperatorOverloading();

#endif // CAMPUS_H
