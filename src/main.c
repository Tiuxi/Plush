#include <stdio.h>
#include "utils/constants.h"
#include "parse/parseInput.h"
#include "utils/error.h"
#include "exec/execCommand.h"
#include "hist/history.h"

int main (int argc, char** argv) {
    // pass compilation
    (void) argc;
    (void) argv;

    int running = 1;
    char buffer[PLUSH_MAX_COMMAND_LENGTH];
    int index = 0;

    plushHistory_check_dir();
    
    putchar('$'); putchar(' ');
    
    while (running) {
        char c = getchar();

        switch (c) {

        // "RETURN"
        case KEY_RETURN:

            // if command is empty, skip
            if (index!=0) {
                buffer[index] = '\0';
                if (strncmp(buffer, "exit", PLUSH_MAX_COMMAND_LENGTH) == 0)
                    running = FALSE;
                else
                    plushExec_execute_command(buffer);
                index = 0;
            }
            putchar('$'); putchar(' ');

            break;
        
        // ctrl + d
        case EOF:
            running = FALSE;
            write(STDOUT_FILENO, "\nexiting\n", 10);
            break;
        

        default:
            buffer[index] = c;
            index++;
            break;
        }
    }

    return 0;
}
