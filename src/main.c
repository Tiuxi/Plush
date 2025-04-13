#include <stdio.h>
#include "constants.h"
#include "parseInput.h"
#include "error.h"
#include "execCommand.h"

int main (int argc, char** argv) {
    // pass compilation
    (void) argc;
    (void) argv;

    int running = 1;
    char buffer[ROOTSH_MAX_COMMAND_LENGTH];
    int index = 0;

    while (running) {
        char c = getchar();

        // user pressed enter
        if (c==10) {
            
            buffer[index] = '\0';
            if (strncmp(buffer, "exit", ROOTSH_MAX_COMMAND_LENGTH) == 0)
                running = FALSE;
            else
                rootshExec_execute_command(buffer);
            index=0;

        }

        // user pressed ctrl + d
        else if (c == EOF) {
            running = FALSE;
        }

        // anything else
        else {
            buffer[index] = c;
            index++;
        }
    }

    return 0;
}
