#include "robot_runtime.hpp"
#include <string>
#include <cstring>

#include <iostream>

#include <sys/select.h> // system header for select() interface
#include <sys/time.h> // system header for interfaces associated with time
#include <unistd.h> // header for POSIX interfaces

#include <cstdio>   // perror()
#include <cstdlib>  // exit(), EXIT_FAILURE

#include "robot_model.hpp"

RobotRuntime::RobotRuntime(){}

void run (RobotModel &robot) {
    reset_terminal();

    #define MAX_BUFFER_SIZE 20
    char command_buffer[MAX_BUFFER_SIZE] = {0};

    fd_set readfds;
    while (true) {

        if (wait_for_stdin(2, 0)) {
            ssize_t bytes_read = read(STDIN_FILENO, command_buffer, MAX_BUFFER_SIZE);
            sanitize_input(command_buffer, bytes_read);

            if (strcmp(command_buffer, "quit") == 0) break;
        }

        handle_command(command_buffer, robot);
        // reset_terminal();
    }
}

void reset_terminal () {
    // Clear terminal (\033[2J) and move cursor to top left (\033[H).
    std::cout << "\033[2J\033[H" << std::flush;

    std::cout << "Robot Runtime Program" << std::endl;
    std::cout << ">> " << std::flush;
}

bool wait_for_stdin (int seconds, int microseconds) {
    // Configure the descriptor set to monitor standard input for readability.
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    // Set the maximum time to wait for standard input to become readable.
    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = microseconds;

    // Wait for standard input to become readable or until the timeout expires.
    int ready = select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &timeout);

    if (ready == -1) {
        perror("select");
        exit(EXIT_FAILURE);
    }

    if (FD_ISSET(STDIN_FILENO, &readfds)) {
        return true;
    }

    return false;
}

void sanitize_input (char* command_buffer, ssize_t &bytes_read) {
    command_buffer[bytes_read - 1] = '\0'; 
}

void handle_command (char* command_buffer, RobotModel &robot) {
    if (strcmp(command_buffer, "move") == 0) {
        robot.configureMovement(100, 5);
        robot.move(1);
        std::cout << "Position: " << robot.getPosition() << std::endl << std::endl;
    }
}