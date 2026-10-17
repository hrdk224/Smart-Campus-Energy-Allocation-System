#include "campus.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

// ============================================================================
// Unit 5: Demonstration of catch(...) and re-throwing an exception
// ============================================================================
void demonstrateCatchAllAndRethrow() {
    cout << "\n--- DEMO: CATCH(...) AND EXCEPTION RE-THROWING (UNIT 5) ---\n";
    try {
        cout << "  [Step 1] Inner function attempting to open an invalid file...\n";
        loadTextReport("non_existent_file_xyz.txt");
    } catch (...) {
        cout << "  [Step 2] Caught exception with catch(...) [catch-all handler]!\n";
        cout << "  [Step 3] Logging failure and re-throwing exception with 'throw;'...\n";
        throw; // Re-throw the exact same active exception to outer caller
    }
}

// Helper function to safely read double input with error handling
double readPositiveDouble(const string& prompt) {
    double val;
    cout << prompt;
    if (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw invalid_argument("Invalid numeric input!");
    }
    if (val < 0.0) {
        throw invalid_argument("Energy value cannot be negative: " + to_string(val));
    }
    return val;
}

int main(int argc, char* argv[]) {
    // =========================================================================
    // Setting up the three campus buildings
    // 1. Campus Clinic (Priority 1 - Critical Medical)
    // 2. Classroom Block (Priority 2 - Academic)
    // 3. Hostel (Priority 3 - Student Housing)
    // =========================================================================
    CampusClinic clinic(1, "Campus Health Clinic", 1, 40.0, 3, true, 8);
    ClassroomBlock classrooms(2, "Main Classroom Block", 2, 50.0, 12);
    Hostel hostel(3, "Mandela Student Hostel", 3, 30.0, 200, 80);

    // Array of base class pointers demonstrating Polymorphism (Unit 2 & Unit 3)
    Building* campus[3] = {&clinic, &classrooms, &hostel};
    const int buildingCount = 3;

    double solarSupply = 40.0;
    double gridSupply = 50.0;
    double totalSupply = calculateTotalSupply(solarSupply, gridSupply); // 90 kWh (Shortage: Demand is 120 kWh)

    // Initial allocation calculation
    allocateCampusEnergy(campus, buildingCount, totalSupply);

    // Non-interactive automated demo mode for scripting / grading tests
    if (argc > 1) {
        string arg = argv[1];
        if (arg == "--demo" || arg == "-d" || arg == "--test") {
            cout << "====================================================================\n";
            cout << " SMART CAMPUS ENERGY ALLOCATION SYSTEM - AUTOMATED RUN\n";
            cout << "====================================================================\n";

            // Scenario 1: Shortage (Built-in)
            cout << "\n[SCENARIO 1: SHORTAGE (Supply 90 kWh < Demand 120 kWh)]\n";
            displayAllocationTable(campus, buildingCount, totalSupply);

            // Scenario 2: Surplus (Supply 150 kWh > Demand 120 kWh)
            cout << "\n[SCENARIO 2: SURPLUS (Supply 150 kWh > Demand 120 kWh)]\n";
            double surplusSupply = calculateTotalSupply(80.0, 70.0);
            allocateCampusEnergy(campus, buildingCount, surplusSupply);
            displayAllocationTable(campus, buildingCount, surplusSupply);

            // Scenario 3: Negative Input Exception Handling
            cout << "\n[SCENARIO 3: EXCEPTION HANDLING (NEGATIVE VALUE REJECTION)]\n";
            try {
                calculateTotalSupply(-20.0, 50.0);
            } catch (const invalid_argument& e) {
                cout << "  [*] Successfully caught expected exception: " << e.what() << "\n";
            }

            // Save text and binary files
            saveTextReport("allocation_report.txt", campus, buildingCount, 80.0, 70.0);
            saveBinaryRecords("allocations.dat", campus, buildingCount);

            // Random binary file access
            cout << "\n[BINARY FILE RANDOM ACCESS DEMO via seekg]\n";
            readBinaryRecordAt("allocations.dat", 0); // Clinic
            readBinaryRecordAt("allocations.dat", 1); // Classrooms

            // Update record via seekp
            cout << "\n[BINARY FILE IN-PLACE UPDATE via seekp]\n";
            updateBinaryRecordAt("allocations.dat", 1, 45.0);
            readBinaryRecordAt("allocations.dat", 1);

            // Small OOP demonstrations
            demonstratePointersAndRefs();
            demonstrateOperatorOverloading();
            demonstrateDiamondInheritance();
            demonstrateNewDelete();
            demonstrateConstructorDestructorOrder();

            cout << "\n====================================================================\n";
            cout << " AUTOMATED RUN COMPLETED SUCCESSFULLY!\n";
            cout << "====================================================================\n";
            return 0;
        }
    }

    // =========================================================================
    // Interactive Console Menu
    // =========================================================================
    int choice = -1;
    while (choice != 0) {
        cout << "\n====================================================================\n";
        cout << "           SMART CAMPUS ENERGY ALLOCATION SYSTEM MENU               \n";
        cout << "====================================================================\n";
        cout << " 1. Show Built-in Shortage Scenario (Solar=40 kWh, Grid=50 kWh -> 90 kWh)\n";
        cout << " 2. Show Surplus Scenario (Solar=80 kWh, Grid=60 kWh -> 140 kWh)\n";
        cout << " 3. Enter Custom Solar and Grid Supply (Test Inputs & Exceptions)\n";
        cout << " 4. Save and View Text Report (allocation_report.txt)\n";
        cout << " 5. Binary File Operations & Random Access (seekg & seekp in allocations.dat)\n";
        cout << " 6. Run Unit 1-3 OOP Demonstrations (Pointers, Diamond, Operators, new/delete)\n";
        cout << " 7. Run Unit 5 Exception Handling Demonstrations (Negative, Missing File, Rethrow)\n";
        cout << " 0. Exit\n";
        cout << "--------------------------------------------------------------------\n";
        cout << " Select option [0-7]: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid number.\n";
            continue;
        }

        try {
            switch (choice) {
                case 1: {
                    // Built-in shortage scenario
                    solarSupply = 40.0;
                    gridSupply = 50.0;
                    totalSupply = calculateTotalSupply(solarSupply, gridSupply);
                    allocateCampusEnergy(campus, buildingCount, totalSupply);
                    displayAllocationTable(campus, buildingCount, totalSupply);
                    break;
                }
                case 2: {
                    // Surplus scenario
                    solarSupply = 80.0;
                    gridSupply = 60.0;
                    totalSupply = calculateTotalSupply(solarSupply, gridSupply);
                    allocateCampusEnergy(campus, buildingCount, totalSupply);
                    displayAllocationTable(campus, buildingCount, totalSupply);
                    break;
                }
                case 3: {
                    // User custom inputs with exception handling
                    cout << "\nEnter Custom Energy Supply:\n";
                    solarSupply = readPositiveDouble("  Enter Solar Energy (kWh): ");
                    gridSupply = readPositiveDouble("  Enter Grid Energy (kWh):  ");
                    totalSupply = calculateTotalSupply(solarSupply, gridSupply);
                    allocateCampusEnergy(campus, buildingCount, totalSupply);
                    displayAllocationTable(campus, buildingCount, totalSupply);
                    break;
                }
                case 4: {
                    // Save and load text report
                    saveTextReport("allocation_report.txt", campus, buildingCount, solarSupply, gridSupply);
                    loadTextReport("allocation_report.txt");
                    break;
                }
                case 5: {
                    // Binary file operations & seekg / seekp
                    cout << "\n--- BINARY FILE OPERATIONS & RANDOM ACCESS (UNIT 4) ---\n";
                    saveBinaryRecords("allocations.dat", campus, buildingCount);

                    cout << "\nChoose record index to read directly via seekg [0=Clinic, 1=Classrooms, 2=Hostel]: ";
                    int idx;
                    if (cin >> idx) {
                        readBinaryRecordAt("allocations.dat", idx);

                        cout << "\nWould you like to update this record's allocation in-place using seekp? (1 for Yes, 0 for No): ";
                        int updateChoice;
                        if (cin >> updateChoice && updateChoice == 1) {
                            double newAlloc = readPositiveDouble("  Enter new allocated kWh: ");
                            updateBinaryRecordAt("allocations.dat", idx, newAlloc);
                            readBinaryRecordAt("allocations.dat", idx);
                        }
                    }
                    break;
                }
                case 6: {
                    // OOP Concept demonstrations
                    demonstratePointersAndRefs();
                    demonstrateOperatorOverloading();
                    demonstrateDiamondInheritance();
                    demonstrateNewDelete();
                    demonstrateConstructorDestructorOrder();
                    demonstrateStringStream(*campus[0]);
                    break;
                }
                case 7: {
                    // Exception handling demonstrations
                    cout << "\n--- EXCEPTION HANDLING DEMONSTRATIONS (UNIT 5) ---\n";

                    // Demo 1: Negative value
                    cout << "1. Testing Negative Value Rejection:\n";
                    try {
                        cout << "   Attempting: calculateTotalSupply(-15.0, 50.0)...\n";
                        calculateTotalSupply(-15.0, 50.0);
                    } catch (const invalid_argument& e) {
                        cout << "   -> Caught std::invalid_argument: " << e.what() << "\n";
                    }

                    // Demo 2: Missing file
                    cout << "\n2. Testing Missing File Error:\n";
                    try {
                        cout << "   Attempting: loadTextReport(\"missing_file.txt\")...\n";
                        loadTextReport("missing_file.txt");
                    } catch (const runtime_error& e) {
                        cout << "   -> Caught std::runtime_error: " << e.what() << "\n";
                    }

                    // Demo 3: catch(...) and rethrow
                    cout << "\n3. Testing catch(...) and Rethrow:\n";
                    try {
                        demonstrateCatchAllAndRethrow();
                    } catch (const exception& e) {
                        cout << "  [Step 4] Outer main() caught re-thrown exception: " << e.what() << "\n";
                    }
                    break;
                }
                case 0: {
                    cout << "\nThank you for using the Smart Campus Energy Allocation System. Goodbye!\n";
                    break;
                }
                default: {
                    cout << "Invalid choice. Please select from the menu.\n";
                    break;
                }
            }
        } catch (const invalid_argument& e) {
            cerr << "\n[INPUT ERROR] " << e.what() << "\n";
        } catch (const runtime_error& e) {
            cerr << "\n[FILE/SYSTEM ERROR] " << e.what() << "\n";
        } catch (const exception& e) {
            cerr << "\n[GENERAL ERROR] " << e.what() << "\n";
        }
    }

    return 0;
}
