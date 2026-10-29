#include "robot_runtime.hpp"

#include <iostream>     // cout, flush
#include <sys/select.h> // select(), fd_set, FD_ZERO(), FD_SET(), struct timeval
#include <unistd.h>     // STDIN_FILENO
#include <cstdio>       // perror()
#include <cstdlib>      // exit(), EXIT_FAILURE
#include <cstring>      // strcmp()
#include <cerrno>       // errno

// #include <sys/time.h> // system header for interfaces associated with time

void RobotRuntime::run() {
    // Clear terminal (\033[2J) and move cursor to top left (\033[H).
    std::cout << "\033[2J\033[H" << std::flush;
    std::cout << "Robot Runtime Program" << std::endl;
    std::cout << ">> " << std::flush;

    #define MAX_BUFFER_SIZE 10
    char command_buffer[MAX_BUFFER_SIZE] = {0};

    while (true) {
        fd_set readable_file_descriptors;
        FD_ZERO(&readable_file_descriptors);
        FD_SET(STDIN_FILENO, &readable_file_descriptors);

        // Set the maximum time to wait for standard input to become readable.
        int seconds = 2;
        int microseconds = 0;
        struct timeval timeout;
        timeout.tv_sec = seconds;
        timeout.tv_usec = microseconds;

        // Wait for standard input to become readable or until the timeout expires.
        int ready = select(STDIN_FILENO + 1, &readable_file_descriptors, nullptr, nullptr, &timeout);

        if (ready == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        // Process standard input if readable.
        if (FD_ISSET(STDIN_FILENO, &readable_file_descriptors)) {
            ssize_t bytes_read = read(STDIN_FILENO, command_buffer, MAX_BUFFER_SIZE);

            // TODO: Improve read() errors and EOF handling.
            if (bytes_read == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            if (bytes_read == 0) {
                break;
            }

            std::cout << "Bytes Read: " << bytes_read << std::endl;

            // Reject commands that exceed max input and discard any remaining characters.
            if (bytes_read == MAX_BUFFER_SIZE) {
                if (command_buffer[bytes_read - 1] != '\n') {
                    char discard;
                    ssize_t discard_bytes_read;

                    while (true) {
                        discard_bytes_read = read(STDIN_FILENO, &discard, 1);

                        if (discard_bytes_read == -1) {
                            perror("read");
                            exit(EXIT_FAILURE);
                        }

                        if (discard_bytes_read == 0) break;
                        if (discard == '\n') break;
                    }

                    std::cout << ">> " << std::flush;
                    continue;  
                }
            }

            // Replace newline with null terminator for string comparison.
            if (command_buffer[bytes_read - 1] == '\n') command_buffer[bytes_read - 1] = '\0';
            else continue;

            std::cout << "\033[2J\033[H" << std::flush;
            std::cout << "Robot Runtime Program" << std::endl;

            // TODO: Improve command handling
            if (strcmp(command_buffer, "quit") == 0) {
                std::cout << "\033[2J\033[H" << std::flush;
                break;
            }
            if (strcmp(command_buffer, "move") == 0) std::cout << "Moving... " << std::endl;
            if (strcmp(command_buffer, "stop") == 0) std::cout << "Idle... " << std::endl;

            std::cout << ">> " << std::flush;

        }
    }
}