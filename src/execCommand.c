#include "execCommand.h"

/**
 * Get every executables directories from the environement variable "PATH"
 */
List getEnvironementsDir()
{
    char *pathstr = getenv(PATHVAR);
    if (pathstr == NULL)
        ASSERT(FALSE);

    int lenPath = strlen(pathstr);

    char *currentDir = (char *)malloc(sizeof(char) * PATH_MAX);
    List paths = rootshList_new(currentDir);

    int currentDirIndex = 0;
    for (int i = 0; i < lenPath; i++)
    {
        if (pathstr[i] == ':')
        {
            currentDir[currentDirIndex] = '\0';
            currentDir = (char *)malloc(sizeof(char) * PATH_MAX);
            rootshList_push(paths, currentDir);
            currentDirIndex = 0;
        }
        else
        {
            currentDir[currentDirIndex] = pathstr[i];
            currentDirIndex++;
        }
    }

    return paths;
}

/**
 * Check in every directory of environement variable "PATH" if the command "command" is in it
 * 
 * @param command The name of the command to search
 * @param paths The list of every paths from the "PATH" env var (in the form of a `rootshList`)
 * @return The full path to the command
 */
char* get_single_command_from_env_paths(char* command, List paths) {
    char* executablePath = (char*)malloc(sizeof(char) * PATH_MAX);
    int found = 0;

    DIR* dir;

    // open every "PATH" directories
    List currentDir = paths;
    while (currentDir != NULL && !found) {
        // open dir
        dir = opendir(currentDir->v);
        if (dir==NULL) {
            // if dir not found, skip to next dir
            if (errno == ENOENT) {
                currentDir = currentDir->next;
                continue;
            }

            // uknown error, stop
            else {
                printf("Errno : %d\n", errno);
                ASSERT(FALSE);
            }
        }

        struct dirent* file = readdir(dir);

        // check for every file in directory
        while (file != NULL && !found) {

            // check file name
            if (strncmp(file->d_name, command, NAME_MAX) == 0){
                found = 1;
                snprintf(executablePath, PATH_MAX, "%s/%s", (char*)currentDir->v, file->d_name);
            }

            // swap to next file in directory
            errno = 0;
            file = readdir(dir);
        }

        ASSERT(closedir(dir) != -1);

        currentDir = currentDir->next;
    }

    if (found==0) {
        free(executablePath);
        return NULL;
    }
    
    return executablePath;
}

// main function
void rootshExec_execute_command(char* commandStr) {
    List paths = getEnvironementsDir();

    // parse the command
    List commands = rootshInput_splitInput(commandStr);

    for (List command=commands; command!=NULL; command=command->next) {
        List currentCommand = command->v;
        char* executable = NULL;

        // Check redirection
        Error errorRedirect = rootshError_new_error();
        if (rootshInput_checkRedirect(currentCommand, errorRedirect) == -1) {
            rootshError_print_error(errorRedirect);
            rootshError_destroy_error(errorRedirect);

            // free everything
            rootshList_destroy2DListAll(commands);
            rootshList_destroyAll(paths);
            return;
        }
        rootshError_destroy_error(errorRedirect);

        // Check file
        if (ISFILE(currentCommand)) {
            executable = (char*)malloc(sizeof(char) * ROOTSH_MAX_ARG_LENGTH);
            snprintf(executable, ROOTSH_MAX_ARG_LENGTH, "%s", (char*)currentCommand->v);
        }

        // Check "PATH" executables
        else {
            executable = get_single_command_from_env_paths(currentCommand->v, paths);
        }

        if (executable==NULL) {
            // set error & print it
            Error err = rootshError_new_error();
            rootshError_set_error_with_argument(err, "Command not found", currentCommand->v);
            rootshError_print_error(err);
            rootshError_destroy_error(err);

            // free everything
            rootshList_destroy2DListAll(commands);
            rootshList_destroyAll(paths);
            return;
        }

        // execute command in a child process
        switch (fork()) {
            // fork error
            case -1:
                rootshError_print_new_error("CRITICAL : fork error");
                return;
                break;
            
            // child process
            case 0:
                int nbArguments = rootshList_size(currentCommand);
                char** arguments = (char**)malloc(sizeof(char*) * (nbArguments+1));

                // set argument list
                List tmp = currentCommand;
                for (int i=0; i<nbArguments; i++) {
                    arguments[i] = tmp->v;
                    tmp = tmp->next;
                }
                arguments[nbArguments] = (char*)NULL; // required for execv

                // execute command
                int exitStatus = execv(executable, arguments);

                // free value
                rootshList_destroy2DListAll(commands);
                rootshList_destroyAll(paths);
                free(executable);
                free(arguments);

                if (exitStatus == -1)
                    exit(errno);
                else
                    exit(0);
                break;
            
            // main process
            default:
                break;
        }

        // wait for the child process to execute
        int childStatus;
        ASSERT(wait(&childStatus) != -1);
        if (WIFEXITED(childStatus)) {

            // check child return code
            switch (WEXITSTATUS(childStatus)) {
            case 0: // no error
                break;
            case EACCES: // permission denied
                rootshError_print_new_error("Permission denied");
                break;
            case ENOENT: // File not found
                rootshError_print_new_error("File not found");
                break;
            case EEXIST: // File exist
                rootshError_print_new_error("File already exist");
                break;
            
            default:
                char errnoNb[5];
                snprintf(errnoNb, 5, "%d", WEXITSTATUS(childStatus));

                Error err = rootshError_new_error();
                rootshError_set_error_with_argument(err, "Unexcpected error", errnoNb);
                rootshError_print_error(err);
                rootshError_destroy_error(err);
                break;
            }
        }

        // free the path of the executable
        free(executable);
    }

    // free everything
    rootshList_destroy2DListAll(commands);
    rootshList_destroyAll(paths);

    return;
}
