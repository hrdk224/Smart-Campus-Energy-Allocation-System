#!/usr/bin/env bash
set -e

# Build if needed
if [ ! -f "build/campus_energy_sim" ]; then
    make build
fi

echo "================================================================================"
echo "          SMART CAMPUS ENERGY ALLOCATION SYSTEM - 10-MINUTE VIVA DEMO          "
echo "================================================================================"
echo " 1. Running Unit & Invariant Test Suite (218 Assertions)..."
./build/campus_energy_tests | grep -E "(TEST SUITE|ALL TESTS PASSED|Passed:)"

echo ""
echo " 2. Running Automated Multi-Scenario Simulation..."
./build/campus_energy_sim --demo | grep -A 25 "EXECUTIVE SUMMARY REPORT"

echo ""
echo "================================================================================"
echo "  [DEMO COMPLETE] Full report saved in reports/campus_energy_report.txt        "
echo "================================================================================"
