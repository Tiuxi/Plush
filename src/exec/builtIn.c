#include "builtIn.h"

int plushBuiltin_check_builtin(List cmd) {
    char* command = cmd->v;

    if (!strncmp(command, "cd", PLUSH_MAX_COMMAND_LENGTH)) {

        char* newPWD = NULL;

        // 1 argument ("cd"), return to home folder
        if (plushList_size(cmd) < 2) {
            newPWD = getenv(VAR_HOME);
        }

        // 2 arguments ("cd", "dir")
        else if (plushList_size(cmd) == 2) {
            newPWD = (char*)cmd->next->v;
        } 
        
        // 3+ arguments, throw error
        else {
            plushError_print_new_error("cd : too many arguments");
        }
        
        if (newPWD != NULL) {
            chdir(newPWD);
        }
        return TRUE;
    }

    if (!strncmp(command, "history", PLUSH_MAX_COMMAND_LENGTH)) {
        int index = (history.index+1) % HISTORY_SIZE;

        while (index != history.index) {
            if (history.hist[index] != NULL) {
                write(STDOUT_FILENO, history.hist[index], PLUSH_MAX_COMMAND_LENGTH);
            }

            index = (index+1) % HISTORY_SIZE;
        }

        return TRUE;
    }

    return FALSE;
}