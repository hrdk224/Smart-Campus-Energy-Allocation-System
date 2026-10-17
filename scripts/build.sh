#!/usr/bin/env bash
set -e

# Detect CMake
CMAKE_BIN=$(which cmake 2>/dev/null || true)
if [ -z "$CMAKE_BIN" ]; then
    STM32_CMAKE="/Applications/STM32CubeIDE.app/Contents/Eclipse/plugins/com.st.stm32cube.ide.mcu.externaltools.cmake.macosaarch64_1.0.100.202605190741/tools/bin/cmake"
    if [ -f "$STM32_CMAKE" ]; then
        CMAKE_BIN="$STM32_CMAKE"
    else
        echo "Error: cmake not found in PATH or standard locations."
        exit 1
    fi
fi

echo "Using CMake: $CMAKE_BIN"
mkdir -p build
"$CMAKE_BIN" -B build -S . -DCMAKE_BUILD_TYPE=Release
"$CMAKE_BIN" --build build

echo "Build successful! Binaries created in build/:"
echo "  - build/campus_energy_sim"
echo "  - build/campus_energy_tests"
