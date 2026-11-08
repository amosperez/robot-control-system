#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include "robot_runtime.hpp"

int main (int argc, char* argv[]) {
    std::cout << "\033[H"; // Move cursor to top left of terminal.
    std::cout << "\033[2J"; // Clear terminal.

    RobotRuntime runtime;
    runtime.run();
    
    return 0;
}