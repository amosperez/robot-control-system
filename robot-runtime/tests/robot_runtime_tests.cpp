#include "robot_runtime.hpp"

#include <unistd.h>     // pipe(), dup(), dup2(), read(), write(), close()
#include <cstdio>       // perror()
#include <cstdlib>      // exit(), EXIT_FAILURE
#include <cassert>      // assert()
#include <cstring>      // strcmp()
#include <string.h>     // strlen()




int main() {

    // Given: Input is available on STDIN_FILENO, and timeout is not nullptr.
    //  When: wait_for_input() waits for input on STDIN_FILENO.
    //  Then: wait_for_input() returns true.
    // {
    //     /* ------------- "in" pipe creation and STDIN_FILENO replacement */

    //     // Create the "in" pipe.
    //     int in[2];
    //     if (pipe(in) == -1) {
    //         perror("pipe");
    //         exit(EXIT_FAILURE);
    //     }

    //     // Create another reference to the resource STDIN_FILENO refers to.
    //     int original_stdin = dup(STDIN_FILENO);
    //     if (original_stdin == -1) {
    //         perror("dup");
    //         exit(EXIT_FAILURE);
    //     }

    //     // Replace the resource referenced by STDIN_FILENO with the one referenced by in[0].
    //     if (dup2(in[0], STDIN_FILENO) == -1) {
    //         perror("dup2");
    //         exit(EXIT_FAILURE);
    //     }

    //     // Remove the in[0] reference. STDIN_FILENO now refers to the same resource, so in[0] is no longer needed.
    //     close(in[0]);

    //     /* ------------- */

    //     const char* command = "quit\n";
    //     if (write(in[1], command, 5) != 5) {
    //         perror("write");
    //         return EXIT_FAILURE;
    //     }

    //     assert(wait_for_input() == true);

    //     close(in[1]);

    //     /* ------------- Restore STDIN_FILENO */

    //     // Replace the resource referenced by STDIN_FILENO with the one referenced by original_stdin.
    //     if (dup2(original_stdin, STDIN_FILENO) == -1) {
    //         perror("dup2");
    //         exit(EXIT_FAILURE);
    //     }
    // }
    
    // // Given: Input is not available on STDIN_FILENO, and timeout is not nullptr.
    // //  When: wait_for_input() waits for input on STDIN_FILENO.
    // //  Then: wait_for_input() returns false.
    // {
    //     /* ------------- "in" pipe creation and STDIN_FILENO replacement */

    //     // Create the "in" pipe.
    //     int in[2];
    //     if (pipe(in) == -1) {
    //         perror("pipe");
    //         exit(EXIT_FAILURE);
    //     }

    //     // Create another reference to the resource STDIN_FILENO refers to.
    //     int original_stdin = dup(STDIN_FILENO);
    //     if (original_stdin == -1) {
    //         perror("dup");
    //         exit(EXIT_FAILURE);
    //     }

    //     // Replace the resource referenced by STDIN_FILENO with the one referenced by in[0].
    //     if (dup2(in[0], STDIN_FILENO) == -1) {
    //         perror("dup2");
    //         exit(EXIT_FAILURE);
    //     }

    //     // Remove the in[0] reference. STDIN_FILENO now refers to the same resource, so in[0] is no longer needed.
    //     close(in[0]);

    //     /* ------------- */

    //     const char* command = "quit\n";
    //     if (write(in[1], command, 5) != 5) {
    //         perror("write");
    //         return EXIT_FAILURE;
    //     }

    //     assert(wait_for_input() == true);

    //     close(in[1]);

    //     /* ------------- Restore STDIN_FILENO */

    //     // Replace the resource referenced by STDIN_FILENO with the one referenced by original_stdin.
    //     if (dup2(original_stdin, STDIN_FILENO) == -1) {
    //         perror("dup2");
    //         exit(EXIT_FAILURE);
    //     }
    // }


    {
        /* ------------- "in" pipe creation and STDIN_FILENO replacement */

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

        int out[2];
        if (pipe(out) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        int original_stdout = dup(STDOUT_FILENO);
        if (original_stdout == -1) {
            perror("dup");
            exit(EXIT_FAILURE);
        }

        if (dup2(out[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        close(out[1]);

        /* ------------- */

        const char* command = "something\n";
        if (write(in[1], command, strlen(command)) != strlen(command)) {
            perror("write");
            return EXIT_FAILURE;
        }

        process_input();

        char output_buffer[50] = {0};

        ssize_t bytes_read = read(out[0], output_buffer, sizeof(output_buffer));
        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (output_buffer[bytes_read - 1] == '\n') {
            output_buffer[bytes_read - 1] = '\0';
        }

        assert(strcmp("output: something", output_buffer) == 0);

    }
}