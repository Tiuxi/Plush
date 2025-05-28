#include <stdio.h>
#include "utils/constants.h"
#include "parse/parseInput.h"
#include "utils/error.h"
#include "exec/execCommand.h"

int main (int argc, char** argv) {
    // pass compilation
    (void) argc;
    (void) argv;

    int running = 1;
    char buffer[PLUSH_MAX_COMMAND_LENGTH];
    int index = 0;

    while (running) {
        char c = getchar();

        // user pressed enter
        if (c==10) {
            
            buffer[index] = '\0';
            if (strncmp(buffer, "exit", PLUSH_MAX_COMMAND_LENGTH) == 0)
                running = FALSE;
            else
                plushExec_execute_command(buffer);
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
