#include "history.h"

History history;
unsigned int HISTORY_SIZE = 2000;

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

    free(histPath);

    return;
}

void plushHistory_load_file() {

    char* histFilePath = (char*)malloc(sizeof(char)*FILENAME_MAX);
    snprintf(histFilePath, FILENAME_MAX, "%s/%s/%s", getenv(VAR_HOME), PATH_HISTDIR, PATH_HISTFILE);

    history.fd = open(histFilePath, O_CREAT | O_RDWR, 0664);
    history.index = 0;
    history.hist = (char**)malloc(sizeof(char*) * HISTORY_SIZE);
    for (unsigned int i=0; i<HISTORY_SIZE; i++) history.hist[i] = NULL;
    history.hist[history.index] = (char*)calloc(PLUSH_MAX_COMMAND_LENGTH, sizeof(char));

    ssize_t bytes_reads, buff_size=16;
    char* buffer = (char*)malloc(sizeof(char) * buff_size);

    // read all file content
    int currentIndex = 0;
    while ((bytes_reads = read(history.fd, buffer, buff_size)) > 0){

        // copy into hist buffer
        for (int i=0; i<bytes_reads; i++) {
            // new command
            if (buffer[i] == '\n') {
                history.index = (history.index+1) % HISTORY_SIZE;
                history.hist[history.index] = (char*)calloc(PLUSH_MAX_COMMAND_LENGTH, sizeof(char));
                currentIndex = 0;
            } else {
                history.hist[history.index][currentIndex] = buffer[i];
                currentIndex++;
            }
        }
    }

    free(buffer);
    free(histFilePath);

    return;
}

void plushHistory_destroy_history() {
    
    for (unsigned int i=0; i<HISTORY_SIZE; i++) {
        if (history.hist[i] != NULL)
            free(history.hist[i]);
    }

    free(history.hist);

    return;
}

void plushHistory_add_command(char* command) {

    int index=0;

    while (command[index] != '\0'){
        history.hist[history.index][index] = command[index];
        index++;
    }

    history.index = (history.index+1) % HISTORY_SIZE;
    history.hist[history.index] = (char*) calloc(PLUSH_MAX_COMMAND_LENGTH, sizeof(char));

    return;
}