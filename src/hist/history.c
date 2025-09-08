#include "history.h"

void plushHistory_check_dir() {

    char* histPath = (char*)malloc(sizeof(char)*FILENAME_MAX);
    snprintf(histPath, FILENAME_MAX, "%s/%s", getenv(VAR_HOME), PATH_HISTDIR);

    printf("%s\n", histPath);
    DIR* histDir = opendir(histPath);

    if (histDir) { // directory exist
        closedir(histDir);
    } else if (errno == ENOENT) { // directory doesn't exist

        ASSERT(mkdir(histPath, MOD_HISTDIR) == 0);

    } else { // unknown error
        plushError_print_new_error("Cannot open history directory");
        printf("%d\n", errno);
    }

    return;
}