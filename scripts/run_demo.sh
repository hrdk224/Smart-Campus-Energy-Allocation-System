#!/usr/bin/env bash
set -e

# Build if needed
if [ ! -f "build/campus_energy_sim" ]; then
    ./scripts/build.sh
fi

echo "=== Running Automated Smart Campus Energy Allocation Demo ==="
./build/campus_energy_sim --demo
