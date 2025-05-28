#include "builtIn.h"

int plushBuiltin_check_builtin(List cmd) {
    char* command = cmd->v;

    if (!strncmp(command, "cd", PLUSH_MAX_COMMAND_LENGTH)) {

        char* newPWD = NULL;

        if (plushList_size(cmd) < 2) { // no argument, return to home folder
            newPWD = getenv(VAR_HOME);
        } else if (plushList_size(cmd) == 2) {
            newPWD = (char*)cmd->next->v;
        } else {
            plushError_print_new_error("cd : too many arguments");
        }
        
        if (newPWD != NULL) {
            printf("%s\n", newPWD);
            chdir(newPWD);
        }
        return TRUE;
    }

    if (!strncmp(command, "history", PLUSH_MAX_COMMAND_LENGTH)) {
        printf("Not implemented yet");
        return TRUE;
    }

    return FALSE;
}