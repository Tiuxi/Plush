#ifndef PLUSH_CONSTANTS
#define PLUSH_CONSTANTS

#define PLUSH_MAX_COMMAND_LENGTH 512
#define PLUSH_MAX_ARG_LENGTH 100
#define PLUSH_MAX_ERROR_LENGTH 256
#define VAR_ENVPATH "PATH"
#define VAR_WORKINGDIR "PWD"
#define VAR_HOME "HOME"

#ifndef KEY_RETURN
#define KEY_RETURN 10
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

// Syscall checks
#include <stdio.h>

#define ASSERT(op)           \
    if((op) == FALSE) {          \
        fprintf(stderr, "Error: line %d, file \"%s\"\n", __LINE__, __FILE__);   \
        fflush(stderr);     \
        exit(EXIT_FAILURE); \
    }

#endif