#include "campus.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

// Function Overloading
double calculateTotalSupply(double solarKwh, double gridKwh) {
    if (solarKwh < 0.0) {
        throw invalid_argument("Solar energy cannot be negative: " + to_string(solarKwh));
    }
    if (gridKwh < 0.0) {
        throw invalid_argument("Grid energy cannot be negative: " + to_string(gridKwh));
    }
    return solarKwh + gridKwh;
}

EnergyAmount calculateTotalSupply(const EnergyAmount& solar, const EnergyAmount& grid) {
    return solar + grid;
}

void allocateCampusEnergy(Building* buildings[], int count, double totalSupplyKwh) {
    if (totalSupplyKwh < 0.0) {
        throw invalid_argument("Total supply cannot be negative: " + to_string(totalSupplyKwh));
    }

    for (int i = 0; i < count; ++i) {
        buildings[i]->setAllocatedKwh(0.0);
    }

    vector<Building*> sortedBuildings;
    for (int i = 0; i < count; ++i) {
        sortedBuildings.push_back(buildings[i]);
    }

    stable_sort(sortedBuildings.begin(), sortedBuildings.end(),
        [](const Building* a, const Building* b) {
            return a->getPriority() < b->getPriority();
        }
    );

    double availableEnergy = totalSupplyKwh;

    for (Building* b : sortedBuildings) {
        if (availableEnergy <= 0.0) {
            b->setAllocatedKwh(0.0);
            continue;
        }

        double requested = b->getRequestedKwh();
        if (availableEnergy >= requested) {
            b->setAllocatedKwh(requested);
            availableEnergy -= requested;
        } else {
            b->setAllocatedKwh(availableEnergy);
            availableEnergy = 0.0;
        }
    }
}

void displayAllocationTable(Building* buildings[], int count, double totalSupplyKwh) {
    cout << "\n================================================================================\n";
    cout << "          SMART CAMPUS ENERGY ALLOCATION REPORT (CURRENT TIME SLOT)            \n";
    cout << "================================================================================\n";
    cout << " Total Supply Available: " << fixed << setprecision(1) << totalSupplyKwh << " kWh\n";
    cout << "--------------------------------------------------------------------------------\n";
    cout << left
         << setw(6)  << "ID"
         << setw(22) << "Building Name"
         << setw(16) << "Category"
         << setw(10) << "Priority"
         << right
         << setw(12) << "Requested"
         << setw(12) << "Allocated"
         << setw(12) << "Remaining"
         << setw(15) << "Status"
         << "\n";
    cout << "--------------------------------------------------------------------------------\n";

    double totalRequested = 0.0;
    double totalAllocated = 0.0;

    for (int i = 0; i < count; ++i) {
        Building* b = buildings[i];
        totalRequested += b->getRequestedKwh();
        totalAllocated += b->getAllocatedKwh();

        string status = "DEFERRED";
        if (b->isFullySatisfied()) {
            status = "FULLY MET";
        } else if (b->getAllocatedKwh() > 0.0) {
            status = "PARTIALLY MET";
        }

        cout << left
             << setw(6)  << b->getId()
             << setw(22) << b->getName()
             << setw(16) << b->getCategory()
             << setw(10) << ("P" + to_string(b->getPriority()))
             << right << fixed << setprecision(1)
             << setw(12) << b->getRequestedKwh()
             << setw(12) << b->getAllocatedKwh()
             << setw(12) << b->getRemainingKwh()
             << setw(15) << status
             << "\n";
    }

    double totalRemaining = (totalRequested - totalAllocated > 0.0) ? (totalRequested - totalAllocated) : 0.0;

    cout << "--------------------------------------------------------------------------------\n";
    cout << left
         << setw(54) << "CAMPUS TOTALS:"
         << right << fixed << setprecision(1)
         << setw(12) << totalRequested
         << setw(12) << totalAllocated
         << setw(12) << totalRemaining
         << "\n";
    cout << "================================================================================\n";

    if (totalRemaining > 0.0) {
        cout << " [!] SHORTAGE NOTICE: Campus demand exceeds supply by "
             << totalRemaining << " kWh. Lower-priority loads deferred.\n";
    } else {
        cout << " [*] SUCCESS: 100% of campus building demands have been satisfied.\n";
    }
}
