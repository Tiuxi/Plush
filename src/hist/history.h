#ifndef PLUSH_HISTORY
#define PLUSH_HISTORY

#include "utils/constants.h"
#include "utils/error.h"
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define PATH_HISTDIR ".config/plush"
#define PATH_HISTFILE "plush_history"
#define MOD_HISTDIR S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH

/**
 * Check if the directory for the history exist
 */
void plushHistory_check_dir();

/**
 * Load the history file, or create it if doesn't exist.  
 * Also allocate and create the history var.
 */
void plushHistory_load_file();

// history global variables
typedef struct s_hist{
    int fd;
    char** hist;
    int index;
}History;
History history;

#endif