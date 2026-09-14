#include "builtIn.h"

int plushBuiltin_check_builtin(List cmd) {
    char* command = cmd->v;
    size_t commandLen = strlen(command);

    if (!strncmp(command, "cd", commandLen)) {
        char* newPWD = NULL;

        // 1 argument ("cd"), return to home folder
        if (plushList_size(cmd) < 2) {
            newPWD = getenv(VAR_HOME);
            if (newPWD == NULL || newPWD[0] == '\0')
                plushError_print_error("$HOME not initialized");
        }

        // 2 arguments ("cd", "dir")
        else if (plushList_size(cmd) == 2) {
            newPWD = (char*)cmd->next->v;
        } 
        
        // 3+ arguments, throw error
        else {
            plushError_print_error("cd : too many arguments");
        }
        
        if (newPWD != NULL && newPWD[0] != '\0') {
            if (chdir(newPWD)==-1) {
                switch (errno) {
                case ENOENT:
                    plushError_print_error("No directory named %s", newPWD);

                    break;
                
                default:
                    plushError_print_error("Unkown error. errno : %d", errno);
                    break;
                }
            };
        }
        return TRUE;
    }

    if (!strncmp(command, "history", commandLen)) {
        if (!isHistoryActivated) return TRUE;
        int index = (history.index+1) % HISTORY_SIZE;

        while (index != history.index) {
            if (history.hist[index] != NULL) {
                ssize_t bytes_written;

                bytes_written = write(STDOUT_FILENO, history.hist[index], strlen(history.hist[index]));
                bytes_written = write(STDOUT_FILENO, "\n", 2);

                (void)bytes_written; // for compiler -Wextra
            }

            index = (index+1) % HISTORY_SIZE;
        }

        return TRUE;
    }

    return FALSE;
}