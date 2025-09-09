#include "history.h"

void plushHistory_check_dir() {

    char* histPath = (char*)malloc(sizeof(char)*FILENAME_MAX);
    snprintf(histPath, FILENAME_MAX, "%s/%s", getenv(VAR_HOME), PATH_HISTDIR);

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

void plushHistory_load_file() {

    char* histFilePath = (char*)malloc(sizeof(char)*FILENAME_MAX);
    snprintf(histFilePath, FILENAME_MAX, "%s/%s/%s", getenv(VAR_HOME), PATH_HISTDIR, PATH_HISTFILE);

    history.fd = open(histFilePath, O_CREAT | O_RDWR, 0664);
    history.index = 0;
    history.hist = (char**)malloc(sizeof(char*) * HISTORY_SIZE);
    for (unsigned int i=0; i<HISTORY_SIZE; i++) history.hist[i] = NULL;
    history.hist[history.index] = (char*)malloc(sizeof(char) * PLUSH_MAX_COMMAND_LENGTH);

    ssize_t bytes_reads, buff_size=16;
    char* buffer = (char*)malloc(sizeof(char) * buff_size);

    // read all file content
    int currentIndex = 0;
    while ((bytes_reads = read(history.fd, buffer, buff_size)) > 0){

        // copy into hist buffer
        for (int i=0; i<bytes_reads; i++) {
            history.hist[history.index][currentIndex] = buffer[i];
            currentIndex++;

            // new command
            if (buffer[i] == '\n') {
                history.index = (history.index+1) % HISTORY_SIZE;
                history.hist[history.index] = (char*)malloc(sizeof(char) * PLUSH_MAX_COMMAND_LENGTH);
            }
        }
    }

    return;
}

void plushHistory_destory_history() {
    
}