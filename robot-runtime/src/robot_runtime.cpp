#include "robot_model.hpp"
#include "robot_runtime.hpp"

#include <iostream>     // cout, flush
#include <chrono>       // steady_clock, milliseconds()
#include <thread>       // this_thread::sleep_for()
#include <sys/select.h> // select(), fd_set, FD_ZERO(), FD_SET(), struct timeval
#include <unistd.h>     // STDIN_FILENO

using namespace std;
using namespace chrono;
using namespace this_thread;

void run () {
    RobotModel robot(123);

    steady_clock::time_point start_time;
    steady_clock::time_point current_time;
    steady_clock::time_point end_time;
    steady_clock::duration work_duration;
    
    duration<double> elasped_time;

    bool done = false;
    while (!done) {
        start_time = steady_clock::now();

        if (wait_for_input() == true) {
            process_input();
        }

        robot.update();

        sleep_for(seconds(3));

        end_time = steady_clock::now();

        elasped_time = end_time - start_time;
    }
}

bool wait_for_input () {
    fd_set readable_file_descriptors;
    FD_ZERO(&readable_file_descriptors);
    FD_SET(STDIN_FILENO, &readable_file_descriptors);

    struct timeval timeout = {2, 0};

    int ready = select(STDIN_FILENO + 1, &readable_file_descriptors, nullptr, nullptr, &timeout);

    if (ready == -1) {
        perror("select");
        exit(EXIT_FAILURE);
    }

    return FD_ISSET(STDIN_FILENO, &readable_file_descriptors);
}

void process_input (RobotModel robot) {
    const int MAX_BUFFER_SIZE = 20;
    char command_buffer[MAX_BUFFER_SIZE] = {0};

    ssize_t bytes_read = read(STDIN_FILENO, command_buffer, MAX_BUFFER_SIZE);

    if (bytes_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    else if (bytes_read == 0) {
        // Handle EOF
        // Emergency shutdown? Graceful shutdown?
    }

    // Reject commands that do not end with newline character. This should handle input that exceeds max input.
    else if (command_buffer[bytes_read - 1] != '\n') {
        while (command_buffer[bytes_read - 1] != '\n') {
            bytes_read = read(STDIN_FILENO, command_buffer, MAX_BUFFER_SIZE);

            if (bytes_read == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            if (bytes_read == 0) {
                // Handle EOF
                // Emergency shutdown? Graceful shutdown?
                break;
            }
        }
        return;
    }
    else { // if (command_buffer[bytes_read - 1] == '\n') {
        command_buffer[bytes_read - 1] = '\0';
    }

    robot.handleCommand(command_buffer);
}

void update () {

}