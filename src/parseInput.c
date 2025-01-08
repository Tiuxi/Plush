#include "parseInput.h"

List rootshInput_splitInput(char* command) {
    int commandLength = strlen(command);

    char* s = (char*)malloc(sizeof(char) * ROOTSH_MAX_ARG_LENGTH);
    List argList = rootshList_new(s); // list with current command arguments
    List commandList = rootshList_new(argList); // list with every command
    List n = argList; // list element of current argument

    int currentIndex = 0;
    for (int i=0; i<commandLength; i++) {

        // if pipe, separate in a new command
        if (currentIndex == 0 && command[i] == '|') {
            if (currentIndex == 0) {
                rootshList_pop(argList);
            }else {
                ((char *)(n->v))[currentIndex] = '\0';
                currentIndex = 0;
            }

            s = (char*)malloc(sizeof(char) * ROOTSH_MAX_ARG_LENGTH);
            argList = rootshList_new(s);
            commandList = rootshList_push(commandList, argList);
            n = argList;

            while (i < commandLength && (command[i] == '|' || command[i] == ' ')){
                i++;
            }
        }
        
        // slice by spaces
        if (command[i] == ' ') {
            ((char*)(n->v))[currentIndex] = '\0';
            currentIndex = 0;
            s = (char*)malloc(sizeof(char) * ROOTSH_MAX_ARG_LENGTH);
            argList = rootshList_push(argList, s);
            n = n->next;
        }

        // else put the char at the end of the string
        else {
            if (currentIndex < ROOTSH_MAX_ARG_LENGTH-1) {
                ((char *)(n->v))[currentIndex] = command[i]; // copy every char
                currentIndex++;
            }
        }
    }
    ((char*)(n->v))[currentIndex] = '\0';

    return commandList;
}

int rootshInput_checkRedirect(List command, Error error) {
    List tmp = command;

    // if redirection before command, raise error
    if (ISREDIRECT(tmp)) {
        rootshError_set_error_with_argument(error, "Syntax error : redirection before command", tmp->v);
        return 0;
    }

    for (tmp=command; tmp!=NULL; tmp=tmp->next) {

        // is a redirection
        if (ISREDIRECT(tmp)) {

            // if there is nothing after, raise error
            if (tmp->next == NULL) {
                rootshError_set_error_with_argument(error, "Syntax error : no file specified for redirection", tmp->v);
                return 0;
            }

            // if there is another redirection right after, raise error

            // if redirection already exist, raise error

        }
    }

    return 1;
}

int rootshInput_isFile(List command) {
    return (((char *)command->v)[0] == '/' ||
            ((char *)command->v)[0] == '~' ||
            ((char *)command->v)[0] == '.');
}