#include "robot_runtime.hpp"

#include <iostream>     // cout, flush
#include <sys/select.h> // select(), fd_set, FD_ZERO(), FD_SET(), struct timeval
#include <unistd.h>     // STDIN_FILENO
#include <cstdio>       // perror()
#include <cstdlib>      // exit(), EXIT_FAILURE
#include <cstring>      // strcmp()
#include <cerrno>       // errno
// #include <sys/time.h> // system header for interfaces associated with time

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
    void wait_for_stdin_readability (fd_set &readable_file_descriptors, struct timeval &timeout) {
        int ready = select(STDIN_FILENO + 1, &readable_file_descriptors, nullptr, nullptr, &timeout);

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

    while (true) {
        fd_set readable_file_descriptors = create_stdin_read_set();

        struct timeval timeout = create_timeout(2, 0);

        wait_for_stdin_readability(readable_file_descriptors, timeout);

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
                std::cout << "quit acknowledged" << std::endl;
                break;
            }
            if (strcmp(command_buffer, "move") == 0) std::cout << "Moving... " << std::endl;
            if (strcmp(command_buffer, "stop") == 0) std::cout << "Idle... " << std::endl;

            std::cout << ">> " << std::flush;

        }
    }
}