#include "robot_runtime.hpp"
#include "robot_model.hpp"

#include <iostream>     // cout, flush
#include <sys/select.h> // select(), fd_set, FD_ZERO(), FD_SET(), struct timeval
#include <unistd.h>     // STDIN_FILENO
#include <cstdio>       // perror()
#include <cstdlib>      // exit(), EXIT_FAILURE
#include <cstring>      // strcmp()
#include <cerrno>       // errno
#include <chrono>       // steady_clock, milliseconds()
// #include <sys/time.h> // system header for interfaces associated with time

using namespace std;
using namespace chrono;

#define MAX_BUFFER_SIZE 10

namespace {
    // Create a timeout specifying how long to wait for standard input to become readable.
    struct timeval create_timeout (int seconds, int microseconds) {
        // Set the maximum time to wait for standard input to become readable
        struct timeval timeout;
        timeout.tv_sec = seconds;
        timeout.tv_usec = microseconds;
        return timeout;
    }

    // Create a file descriptor set for monitoring standard input readability.
    fd_set create_stdin_read_set () {
        fd_set readable_file_descriptors;
        FD_ZERO(&readable_file_descriptors);
        FD_SET(STDIN_FILENO, &readable_file_descriptors);
        return readable_file_descriptors;
    }

    // Wait for standard input to become readable or until the timeout expires.
    void wait_for_stdin_readability (fd_set &readable_file_descriptors, struct timeval *timeout) {
        int ready = select(STDIN_FILENO + 1, &readable_file_descriptors, nullptr, nullptr, timeout);

        if (ready == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        }
    }

    // Read bytes from standard input.
    ssize_t read_bytes_from_stdin (char *buffer, size_t buffer_size) {
        ssize_t bytes_read = read(STDIN_FILENO, buffer, buffer_size);

        // TODO: Improve read() errors.
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        return bytes_read;
    }
}

void RobotRuntime::run() {
    char command_buffer[MAX_BUFFER_SIZE] = {0};
    RobotModel robot(123);

    seconds interval(5);
    steady_clock::time_point next;
    steady_clock::time_point now;
    steady_clock::duration time_left; 
    struct timeval timeout;

    int i = 0;

    while (true) {
        fd_set readable_file_descriptors = create_stdin_read_set();

        if (robot.getState() == RobotState::Moving) {
            now = steady_clock::now();
            time_left = next - now;
            if (time_left < seconds(0)) {
                time_left = seconds(0);
            }

            timeout = create_timeout(duration_cast<seconds>(time_left).count(), 0);
            wait_for_stdin_readability(readable_file_descriptors, &timeout);
        }
        else if (robot.getState() == RobotState::Idle) {
            wait_for_stdin_readability(readable_file_descriptors, nullptr);
        }

        // Process standard input if readable.
        if (FD_ISSET(STDIN_FILENO, &readable_file_descriptors)) {
            
            ssize_t bytes_read = read_bytes_from_stdin(command_buffer, MAX_BUFFER_SIZE);
            if (bytes_read == 0) break;

            // Reject commands that exceed max input and discard any remaining characters.
            if (bytes_read == MAX_BUFFER_SIZE) {
                if (command_buffer[bytes_read - 1] != '\n') {
                    char discard = '\0';
                    ssize_t discard_bytes_read;

                    while (true) {
                        discard_bytes_read = read_bytes_from_stdin(&discard, 1);
                        if (discard_bytes_read == 0) return;
                        if (discard == '\n') break;
                    }
                    continue;  
                }
            }

            // Replace newline with null terminator for string comparison.
            if (command_buffer[bytes_read - 1] == '\n') command_buffer[bytes_read - 1] = '\0';
            else continue;

            // TODO: Improve command handling
            if (strcmp(command_buffer, "quit") == 0) {
                cout << "quit acknowledged" << endl;
                break;
            }
            else if (strcmp(command_buffer, "idle") == 0 && (robot.getState() != RobotState::Idle)) {
                robot.setState(RobotState::Idle);
                cout << "idle acknowledged" << endl;
                continue;
            }
            else if (strcmp(command_buffer, "moving") == 0 && (robot.getState() != RobotState::Moving)) {
                robot.setState(RobotState::Moving);
                cout << "moving acknowledged" << endl;
                next = steady_clock::now();
            }
        }

        if (robot.getState() == RobotState::Moving && now >= next) {
            steady_clock::time_point now = steady_clock::now();

            microseconds difference = duration_cast<microseconds>(now - next);

            cout << "Deadline difference: " << difference.count() << " us\n";

            cout << i++ << " moving" << endl;

            next += interval;
        }
    }
}