#!/usr/bin/env bash
set -e

# Build if needed
if [ ! -f "build/campus_energy_tests" ]; then
    ./scripts/build.sh
fi

echo "=== Running Smart Campus Unit and Invariant Tests ==="
./build/campus_energy_tests
