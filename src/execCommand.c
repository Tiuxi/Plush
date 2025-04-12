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

char* get_single_command_from_paths(List command, List paths) {
    char* executablePath = (char*)malloc(sizeof(char) * PATH_MAX);
    int found = 0;

    // if is a file, go to that location
    if (ISFILE(command)) {
        
    }

    // if not a file, check for executables in "PATH"
    else {
        DIR* dir;

        // open every "PATH" directories
        List currentDir = paths;
        while (currentDir != NULL && !found) {
            // open dir
            dir = opendir(currentDir->v);
            if (dir==NULL) {
                // if dir not found, skip to next dir
                if (errno == 2) {
                    currentDir = currentDir->next;
                    continue;
                }

                // uknown error, stop
                else ASSERT(FALSE);
            }

            struct dirent* file = readdir(dir);
            //struct stat st;

            // check for every file in directory
            while (file != NULL && !found) {

                // check file name
                if (strncmp(file->d_name, command->v, NAME_MAX) == 0){
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
    }

    if (found==0) {
        free(executablePath);
        return NULL;
    }
    
    return executablePath;
}

void rootshExec_execute_command(char* command) {
    List paths = getEnvironementsDir();

    // parse the command
    List commands = rootshInput_splitInput(command);

    for (List command=commands; command!=NULL; command=command->next) {
        List currentCommand = command->v;

        // Check redirection
        Error errorRedirect = rootshError_new_error();
        if (rootshInput_checkRedirect(currentCommand, errorRedirect) == -1) {
            rootshError_print_error(errorRedirect);
            rootshError_destroy_error(errorRedirect);
            return;
        }

        // Check file

        // Check "PATH" executables
        char *executable = get_single_command_from_paths(currentCommand, paths);

        if (executable==NULL) {
            // set error & print it
            rootshError_print_new_error("Command not found");

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
                char** arguments = (char**)malloc(sizeof(char*) * nbArguments+1);

                // set argument list
                List tmp = currentCommand;
                for (int i=0; i<nbArguments; i++) {
                    arguments[i] = tmp->v;
                    tmp = tmp->next;
                    printf("%s\n", arguments[i]);
                }
                arguments[nbArguments] = (char*)NULL; // required for execv

                // execute command
                int exitStatus = execv(executable, arguments);

                // free values
                rootshList_destroy2DListAll(command);

                exit(exitStatus);
                break;
            
            // main process
            default:
                break;
        }

        // wait for the child process to execute
        int childStatus;
        ASSERT(wait(&childStatus) != -1);
        if (WIFEXITED(childStatus) && WEXITSTATUS(childStatus) == -1) {
            rootshError_print_new_error("An unexpected error happened while executing the command");
            return;
        }
        printf("%d\n", WEXITSTATUS(childStatus));

        // free the path of the executable
        free(executable);
    }

    // free everything
    rootshList_destroy2DListAll(commands);

    return;
}
