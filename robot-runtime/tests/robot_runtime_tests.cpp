#include "robot_runtime.hpp"
#include <cassert>

#include <unistd.h>     // pipe(), dup(), dup2(), read(), write(), close()
#include <cstdio>       // perror()
#include <cstdlib>      // exit(), EXIT_FAILURE
#include <cerrno>       // errno
#include <cstring>      // strcmp()
#include <iostream>     // cout, flush

int main () {
    {
        // Create the "in" pipe.
        int in[2];
        if (pipe(in) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        // Create another reference to the resource STDIN_FILENO refers to.
        int original_stdin = dup(STDIN_FILENO);
        if (original_stdin == -1) {
            perror("dup");
            exit(EXIT_FAILURE);
        }

        // Replace the resource referenced by STDIN_FILENO with the one referenced by in[0].
        if (dup2(in[0], STDIN_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        // Remove the in[0] reference. STDIN_FILENO now refers to the same resource, so in[0] is no longer needed.
        close(in[0]);

        /* ------------- "out" pipe creation and STDOUT_FILENO replacement */

        // Create the "out" pipe.
        int out[2];
        if (pipe(out) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        // Create another reference to the resource STDOUT_FILENO refers to.
        int original_stdout = dup(STDOUT_FILENO);
        if (original_stdout == -1) {
            perror("dup");
            exit(EXIT_FAILURE);
        }

        // Replace the resource referenced by STDOUT_FILENO with the one referenced by in[1].
        if (dup2(out[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        // Remove the out[1] reference. STDOUT_FILENO now refers to the same resource, so out[1] is no longer needed.
        close(out[1]);

        /* ------------- */

        const char* command = "quit\n";
        if (write(in[1], command, 5) != 5) {
            perror("write");
            return EXIT_FAILURE;
        }
        close(in[1]);
        /* ------------- */
        
        RobotRuntime runtime;
        runtime.run();
        std::cout << std::flush;

        /* ------------- */
        char output_buffer[50] = {0};
        ssize_t bytes_read = read(out[0], output_buffer, sizeof(output_buffer) - 1);
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        output_buffer[bytes_read] = '\0';

        close(out[0]);

        if (dup2(original_stdin, STDIN_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        close(original_stdin);

        if (dup2(original_stdout, STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        close(original_stdout);

        assert(strcmp(output_buffer, "quit acknowledged\n") == 0);
    }
}
