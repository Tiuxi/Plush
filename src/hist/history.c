#include "history.h"

History history;
unsigned int HISTORY_SIZE = 2000;

void plushHistory_check_dir() {

    char* histPath = (char*)malloc(sizeof(char)*FILENAME_MAX);
    const char* home = getenv(VAR_HOME);
    if (home == NULL) {
        plushError_print_new_warn("$HOME not initialized. History will not be activated");
        return;
    }

    snprintf(histPath, FILENAME_MAX, "%s/%s", home, PATH_HISTDIR);

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
    const char* home = getenv(VAR_HOME);
    if (home == NULL) return;

    snprintf(histFilePath, FILENAME_MAX, "%s/%s/%s", home, PATH_HISTDIR, PATH_HISTFILE);

    history.fd = open(histFilePath, O_CREAT | O_RDWR, MOD_HISTFILE);
    history.index = 0;
    history.hist = (char**)malloc(sizeof(char*) * HISTORY_SIZE);
    for (unsigned int i=0; i<HISTORY_SIZE; i++) history.hist[i] = NULL;
    history.hist[history.index] = (char*)calloc(PLUSH_BASE_COMMAND_LENGTH, sizeof(char));

    ssize_t bytes_reads, buff_size=64;
    char* buffer = (char*)malloc(sizeof(char) * buff_size);

    // read all file content
    int currentIndex = 0;
    while ((bytes_reads = read(history.fd, buffer, buff_size)) > 0){

        // copy into hist buffer
        for (int i=0; i<bytes_reads; i++) {
            // new command
            if (buffer[i] == '\n') {
                history.index = (history.index+1) % HISTORY_SIZE;

                if (history.hist[history.index] != NULL)
                    free(history.hist[history.index]);
                history.hist[history.index] = (char*)calloc(PLUSH_BASE_COMMAND_LENGTH, sizeof(char));
                currentIndex = 0;
            } else if (currentIndex < PLUSH_BASE_COMMAND_LENGTH) {
                history.hist[history.index][currentIndex] = buffer[i];
                currentIndex++;
            }
        }
    }

    free(buffer);
    free(histFilePath);
    close(history.fd);

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

void plushHistory_add_command(const char* command) {
    // check if same command than before
    char* previousCommand = history.hist[(history.index - 1 + HISTORY_SIZE) % HISTORY_SIZE];
    if (previousCommand != NULL && 
        !strncmp(previousCommand, command, max(strlen(previousCommand), strlen(command)))) {

        return;
    }

    int index=0;

    while (command[index] != '\0' && index<PLUSH_BASE_COMMAND_LENGTH){
        history.hist[history.index][index] = command[index];
        index++;
    }

    history.index = (history.index+1) % HISTORY_SIZE;
    if (history.hist[history.index] != NULL)
        free(history.hist[history.index]);
    history.hist[history.index] = (char*)calloc(PLUSH_BASE_COMMAND_LENGTH, sizeof(char));

    return;
}

void plushHistory_save_to_file() {

    char* histFilePath = (char*)malloc(sizeof(char) * FILENAME_MAX);
    const char* home = getenv(VAR_HOME);
    if (home == NULL) return;

    snprintf(histFilePath, FILENAME_MAX, "%s/%s/%s", home, PATH_HISTDIR, PATH_HISTFILE);

    history.fd = open(histFilePath, O_TRUNC | O_WRONLY | O_CREAT, MOD_HISTFILE);

    int index = (history.index + 1) % HISTORY_SIZE;

    while (index != history.index) {
        if (history.hist[index] != NULL) {
            if (write(history.fd, history.hist[index], strlen(history.hist[index])) < 0
            || write(history.fd, "\n", 1) < 0) {
                plushError_print_new_warn("Cannot write to history file");
                break;
            }
        }

        index = (index + 1) % HISTORY_SIZE;
    }

    close(history.fd);
    free(histFilePath);

    return;
}