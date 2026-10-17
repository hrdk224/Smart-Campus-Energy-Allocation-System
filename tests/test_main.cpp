#include "TestHarness.hpp"

// Test suite runners declared in separate test units
void runBuildingTests();
void runSourceTests();
void runEngineTests();
void runPolicyTests();
void runInvariantTests();
void runEducationalOOPTests();

int main() {
    std::cout << "=======================================================\n";
    std::cout << " RUNNING SMART CAMPUS ENERGY ALLOCATION TEST SUITE     \n";
    std::cout << "=======================================================\n";

    runBuildingTests();
    runSourceTests();
    runPolicyTests();
    runEngineTests();
    runInvariantTests();
    runEducationalOOPTests();

    return smartcampus::test::TestRunner::instance().report();
}
