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

#define PATH_HISTDIR ".config/plush"
#define PATH_HISTFILE "plush_history"
#define MOD_HISTDIR S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH

void plushHistory_check_dir();

#endif