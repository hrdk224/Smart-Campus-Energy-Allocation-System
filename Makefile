# Convenience Makefile for Smart Campus Energy Allocation System
# Detects system cmake or falls back to bundled macOS cmake if needed

CMAKE_BIN := $(shell which cmake 2>/dev/null)
ifeq ($(CMAKE_BIN),)
    STM32_CMAKE := /Applications/STM32CubeIDE.app/Contents/Eclipse/plugins/com.st.stm32cube.ide.mcu.externaltools.cmake.macosaarch64_1.0.100.202605190741/tools/bin/cmake
    ifneq ($(wildcard $(STM32_CMAKE)),)
        CMAKE_BIN := $(STM32_CMAKE)
    else
        CMAKE_BIN := cmake
    endif
endif

BUILD_DIR := build

.PHONY: all config build clean test run demo help

all: build

config:
	@mkdir -p $(BUILD_DIR)
	@$(CMAKE_BIN) -B $(BUILD_DIR) -S . -DCMAKE_BUILD_TYPE=Release

build: config
	@$(CMAKE_BIN) --build $(BUILD_DIR)

test: build
	@echo "=== Running Smart Campus Energy Allocation Test Suite ==="
	@cd $(BUILD_DIR) && ./campus_energy_tests

run: build
	@echo "=== Launching Smart Campus Energy Allocation Console UI ==="
	@./$(BUILD_DIR)/campus_energy_sim

demo: build
	@echo "=== Running Automated End-to-End Scenarios Demo ==="
	@./$(BUILD_DIR)/campus_energy_sim --demo

presentation: build
	@./scripts/quick_presentation_demo.sh

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Cleaned build directory."

help:
	@echo "Available targets:"
	@echo "  make         - Configure and build all binaries"
	@echo "  make test    - Run complete unit and invariant test suite"
	@echo "  make run     - Run interactive console simulation"
	@echo "  make demo    - Run non-interactive 4-scenario simulation demo"
	@echo "  make clean   - Remove build directory"
