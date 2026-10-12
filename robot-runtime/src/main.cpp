#include <string>
#include <iostream>
#include <chrono>
#include <thread>

int main (int argc, char* argv[]) {
    std::cout << "\033[H"; // Move cursor to top left of terminal.
    std::cout << "\033[2J"; // Clear terminal.
    
    return 0;
}