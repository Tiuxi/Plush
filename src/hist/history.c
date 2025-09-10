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

    history.fd = open(histFilePath, O_CREAT | O_RDONLY, 0664);
    history.index = 0;
    history.hist = (char**)malloc(sizeof(char*) * HISTORY_SIZE);
    for (unsigned int i=0; i<HISTORY_SIZE; i++) history.hist[i] = NULL;
    history.hist[history.index] = (char*)calloc(PLUSH_MAX_COMMAND_LENGTH, sizeof(char));

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

                if (history.hist[history.index] == NULL)
                    history.hist[history.index] = (char*)calloc(PLUSH_MAX_COMMAND_LENGTH, sizeof(char));
                else
                    for (int i=0; i<PLUSH_MAX_COMMAND_LENGTH; i++) 
                        history.hist[history.index][i] = 0;
                currentIndex = 0;
            } else {
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

    while (command[index] != '\0'){
        history.hist[history.index][index] = command[index];
        index++;
    }

    history.index = (history.index+1) % HISTORY_SIZE;
    if (history.hist[history.index] == NULL)
        history.hist[history.index] = (char*)calloc(PLUSH_MAX_COMMAND_LENGTH, sizeof(char));
    else
        for (int i = 0; i < PLUSH_MAX_COMMAND_LENGTH; i++)
            history.hist[history.index][i] = 0;

    return;
}

void plushHistory_save_to_file() {

    char* histFilePath = (char*)malloc(sizeof(char) * FILENAME_MAX);
    snprintf(histFilePath, FILENAME_MAX, "%s/%s/%s", getenv(VAR_HOME), PATH_HISTDIR, PATH_HISTFILE);
    
    history.fd = open(histFilePath, O_TRUNC | O_WRONLY);

    int index = (history.index + 1) % HISTORY_SIZE;

    while (index != history.index) {
        if (history.hist[index] != NULL) {
            write(history.fd, history.hist[index], PLUSH_MAX_COMMAND_LENGTH);
            write(history.fd, "\n", 2);
        }

        index = (index + 1) % HISTORY_SIZE;
    }

    close(history.fd);
    free(histFilePath);

    return;
}