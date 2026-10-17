#include "campus.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <stdexcept>

using namespace std;

// ============================================================================
// Text File Operations (Unit 4 & Unit 5)
// Uses ofstream to write and ifstream to read
// ============================================================================

void saveTextReport(const string& filename, Building* buildings[], int count, double solar, double grid) {
    // Open text file for writing
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw runtime_error("Failed to open text file for writing: " + filename);
    }

    double totalRequested = 0.0;
    double totalAllocated = 0.0;

    outFile << "====================================================================\n";
    outFile << "       SMART CAMPUS ENERGY ALLOCATION SYSTEM - TEXT REPORT          \n";
    outFile << "====================================================================\n";
    outFile << fixed << setprecision(1);
    outFile << "Solar Generation: " << solar << " kWh\n";
    outFile << "Power Grid Supply: " << grid << " kWh\n";
    outFile << "Total Supply:     " << (solar + grid) << " kWh\n";
    outFile << "--------------------------------------------------------------------\n";
    outFile << left
            << setw(6)  << "ID"
            << setw(20) << "Name"
            << setw(10) << "Priority"
            << right
            << setw(10) << "Requested"
            << setw(10) << "Allocated"
            << setw(10) << "Remaining"
            << "\n";
    outFile << "--------------------------------------------------------------------\n";

    for (int i = 0; i < count; ++i) {
        Building* b = buildings[i];
        totalRequested += b->getRequestedKwh();
        totalAllocated += b->getAllocatedKwh();

        outFile << left
                << setw(6)  << b->getId()
                << setw(20) << b->getName()
                << setw(10) << ("P" + to_string(b->getPriority()))
                << right
                << setw(10) << b->getRequestedKwh()
                << setw(10) << b->getAllocatedKwh()
                << setw(10) << b->getRemainingKwh()
                << "\n";
    }

    outFile << "--------------------------------------------------------------------\n";
    outFile << left << setw(36) << "TOTALS:"
            << right
            << setw(10) << totalRequested
            << setw(10) << totalAllocated
            << setw(10) << (totalRequested - totalAllocated)
            << "\n";
    outFile << "====================================================================\n";

    outFile.close();
    cout << "  [*] Text report saved successfully to: " << filename << "\n";
}

void loadTextReport(const string& filename) {
    // Open text file for reading
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        throw runtime_error("Failed to open text file for reading: " + filename);
    }

    cout << "\n>>> READING SAVED TEXT REPORT FROM [" << filename << "] <<<\n";
    string line;
    while (getline(inFile, line)) {
        cout << line << "\n";
    }
    inFile.close();
}

// ============================================================================
// Binary File Operations & Random Access (Unit 4)
//
// Demonstrates:
// - Fixed-size binary records
// - seekg() to position the read pointer directly at record N
// - seekp() to position the write pointer directly at record N
// - Explaining WHY fixed-size records are required:
//   Fixed record size enables O(1) random access: byte_offset = index * sizeof(BuildingRecord)
// ============================================================================

void saveBinaryRecords(const string& filename, Building* buildings[], int count) {
    ofstream binFile(filename, ios::binary | ios::trunc);
    if (!binFile.is_open()) {
        throw runtime_error("Failed to open binary file for writing: " + filename);
    }

    for (int i = 0; i < count; ++i) {
        BuildingRecord rec;
        rec.id = buildings[i]->getId();
        strncpy(rec.name, buildings[i]->getName().c_str(), sizeof(rec.name) - 1);
        rec.name[sizeof(rec.name) - 1] = '\0';
        rec.priority = buildings[i]->getPriority();
        rec.requestedKwh = buildings[i]->getRequestedKwh();
        rec.allocatedKwh = buildings[i]->getAllocatedKwh();
        rec.remainingKwh = buildings[i]->getRemainingKwh();

        // Write raw binary bytes of the fixed-size struct
        binFile.write(reinterpret_cast<const char*>(&rec), sizeof(BuildingRecord));
    }

    binFile.close();
    cout << "  [*] Saved " << count << " fixed-size binary records ("
         << sizeof(BuildingRecord) << " bytes each) to " << filename << "\n";
}

void readBinaryRecordAt(const string& filename, int recordIndex) {
    ifstream binFile(filename, ios::binary);
    if (!binFile.is_open()) {
        throw runtime_error("Failed to open binary file for reading: " + filename);
    }

    // Determine total records by seeking to the end
    binFile.seekg(0, ios::end);
    streampos fileSize = binFile.tellg();
    int totalRecords = static_cast<int>(fileSize / sizeof(BuildingRecord));

    if (recordIndex < 0 || recordIndex >= totalRecords) {
        binFile.close();
        throw out_of_range("Invalid record index: " + to_string(recordIndex) +
                           " (Total records in file: " + to_string(totalRecords) + ")");
    }

    // =========================================================================
    // RANDOM ACCESS READ USING seekg:
    // Move get-pointer directly to byte offset: recordIndex * sizeof(BuildingRecord)
    // =========================================================================
    streampos offset = static_cast<streampos>(recordIndex) * sizeof(BuildingRecord);
    binFile.seekg(offset, ios::beg);

    BuildingRecord rec;
    binFile.read(reinterpret_cast<char*>(&rec), sizeof(BuildingRecord));
    binFile.close();

    cout << "\n>>> RANDOM ACCESS READ via seekg(" << offset << ", ios::beg) <<<\n";
    cout << "  Record Index:     " << recordIndex << "\n";
    cout << "  Building ID:      " << rec.id << "\n";
    cout << "  Building Name:    " << rec.name << "\n";
    cout << "  Priority:         P" << rec.priority << "\n";
    cout << fixed << setprecision(1);
    cout << "  Requested Energy: " << rec.requestedKwh << " kWh\n";
    cout << "  Allocated Energy: " << rec.allocatedKwh << " kWh\n";
    cout << "  Remaining Demand: " << rec.remainingKwh << " kWh\n";
}

void updateBinaryRecordAt(const string& filename, int recordIndex, double newAllocatedKwh) {
    if (newAllocatedKwh < 0.0) {
        throw invalid_argument("Updated allocation cannot be negative!");
    }

    // Open file with fstream (both read and write in binary mode)
    fstream file(filename, ios::in | ios::out | ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Failed to open binary file for read/write: " + filename);
    }

    // Seek to the record to read current requested value
    streampos offset = static_cast<streampos>(recordIndex) * sizeof(BuildingRecord);
    file.seekg(offset, ios::beg);

    BuildingRecord rec;
    file.read(reinterpret_cast<char*>(&rec), sizeof(BuildingRecord));
    if (!file) {
        file.close();
        throw out_of_range("Failed to read record at index: " + to_string(recordIndex));
    }

    // Update allocated and remaining
    rec.allocatedKwh = newAllocatedKwh;
    rec.remainingKwh = (rec.requestedKwh > newAllocatedKwh) ? (rec.requestedKwh - newAllocatedKwh) : 0.0;

    // =========================================================================
    // RANDOM ACCESS WRITE USING seekp:
    // Move put-pointer directly to the record offset and overwrite in-place
    // =========================================================================
    file.seekp(offset, ios::beg);
    file.write(reinterpret_cast<const char*>(&rec), sizeof(BuildingRecord));
    file.close();

    cout << "  [*] Record #" << recordIndex << " (" << rec.name
         << ") updated in-place via seekp(" << offset << ") to "
         << newAllocatedKwh << " kWh\n";
}

// ============================================================================
// Stringstream Demonstration (Unit 4)
// Formats in-memory strings using stream syntax
// ============================================================================
void demonstrateStringStream(const Building& b) {
    stringstream ss;
    ss << "BuildingLog: [ID=" << b.getId()
       << "] Name=\"" << b.getName()
       << "\" Priority=P" << b.getPriority()
       << " Requested=" << fixed << setprecision(1) << b.getRequestedKwh() << " kWh";

    string logMessage = ss.str(); // Extract string from stringstream
    cout << "  [Stringstream Output] " << logMessage << "\n";
}
