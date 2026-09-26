#ifndef PLUSH_PARSEINPUT
#define PLUSH_PARSEINPUT

#include <string.h>
#include <stdlib.h>
#include "utils/list.h"
#include "utils/constants.h"
#include "utils/error.h"

#define ISSTDIN(arg) \
    (strncmp((char *)arg->v, "<", PLUSH_MAX_ARG_LENGTH) == 0)

#define ISSTDOUT(arg) \
    (strncmp((char *)arg->v, ">", PLUSH_MAX_ARG_LENGTH) == 0)  ||   \
    (strncmp((char *)arg->v, ">>", PLUSH_MAX_ARG_LENGTH) == 0) ||  \
    (strncmp((char *)arg->v, "1>", PLUSH_MAX_ARG_LENGTH) == 0) ||   \
    (strncmp((char *)arg->v, "1>>", PLUSH_MAX_ARG_LENGTH) == 0)

#define ISSTDERR(arg) \
    (strncmp((char *)arg->v, "2>", PLUSH_MAX_ARG_LENGTH) == 0) || \
    (strncmp((char *)arg->v, "2>>", PLUSH_MAX_ARG_LENGTH) == 0)

#define ISREDIRECT(arg) \
    ISSTDERR(arg) || ISSTDOUT(arg) || ISSTDIN(arg)

bool PlushToken_isFile(char* string);


    /************************ FUNCTIONS ************************/


/**
 * Split a string into an list of every command by separating at every pipe.
 * Each command is a list of char separated by spaces
 *
 * @short Split a string into an array
 * @param command       The string to split
 * @return The list of commands
 */
List plushInput_splitInput(char *command);

/**
 * Check in the List "command" if there are redirection and they are correctly made, if not return error message in argument `error`
 * 
 * @param command       The list of argument to check
 * @return 0 if the command is correctly redirected, -1 else
 */
int plushInput_checkRedirect(List command);

#endif