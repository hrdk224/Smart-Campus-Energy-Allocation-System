#include "smartcampus/io/ConsoleMenu.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    try {
        if (argc > 1) {
            std::string arg = argv[1];
            if (arg == "--demo" || arg == "-d" || arg == "--test") {
                return smartcampus::ConsoleMenu::runAutomatedDemo();
            } else if (arg == "--help" || arg == "-h") {
                std::cout << "Usage: ./campus_energy_sim [OPTIONS]\n"
                          << "Options:\n"
                          << "  --demo, -d   Run non-interactive automated 4-scenario demonstration\n"
                          << "  --help, -h   Show this help message\n"
                          << "  (no args)    Launch interactive console menu\n";
                return 0;
            }
        }

        smartcampus::ConsoleMenu menu;
        menu.runInteractive();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n[FATAL SYSTEM ERROR] " << e.what() << "\n";
        return 1;
    }
}
