#include <string>
#include <iostream>

#include <sys/select.h> // system header for select() interface
#include <sys/time.h> // system header for interfaces associated with time
#include <unistd.h> // header for POSIX interfaces

#include <cstdio>   // perror()
#include <cstdlib>  // exit(), EXIT_FAILURE


int main (int argc, char* argv[]) {
    // Clear terminal (\033[2J) and move cursor to top left (\033[H).
    std::cout << "\033[2J\033[H" << std::flush;

    std::cout << "Robot Runtime Program" << std::endl;
    std::cout << ">> " << std::flush;

    #define MAX_BUFFER_SIZE 20
    char command_line_buffer[MAX_BUFFER_SIZE] = {0};

    while (true) {
        // Configure the descriptor set to monitor standard input for readability.
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        // Set the maximum time to wait for standard input to become readable.
        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        // Wait for standard input to become readable or until the timeout expires.
        int ready = select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &timeout);

        if (ready == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        // TODO: Parse the received input and execute the corresponding command.
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            ssize_t bytes_read = read(STDIN_FILENO, command_line_buffer, MAX_BUFFER_SIZE);

            command_line_buffer[bytes_read - 1] = '\0';
            std::cout << "\033[H" << "\033[2J" << std::flush;
            std::cout << "Robot Runtime Program" << std::endl;
            std::cout << ">> " << std::flush;
        }
    }
    
    return 0;
}