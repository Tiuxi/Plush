#include "utils/error.h"

Error plushError_new_error() {
    Error err = (Error)malloc(sizeof(char*));
    *err = NULL;
    return err;
}

void plushError_destroy_error(Error err) {
    if (*err != NULL)
        free(*err);
    
    free(err);
}

void plushError_set_error_message(Error err, const char *message) {
    char* tmp = (char*)calloc(PLUSH_MAX_ERROR_LENGTH, sizeof(char));
    snprintf(tmp, PLUSH_MAX_ERROR_LENGTH, "[%sError%s] %s\n", COLOR_REDBOLD, COLOR_BASE, message);
    *err = tmp;
}

void plushError_set_error_with_argument(Error err, const char* message, char* arg) {
    char* tmp = (char*)calloc(PLUSH_MAX_ERROR_LENGTH, sizeof(char));
    snprintf(tmp, PLUSH_MAX_ERROR_LENGTH, "[%sError%s] %s : %s\n", COLOR_REDBOLD, COLOR_BASE, message, arg);
    *err = tmp;
}

void plushError_print_error(Error err) {
    ssize_t bytes_written = write(STDERR_FILENO, *err, PLUSH_MAX_ERROR_LENGTH);
    (void)bytes_written;
}

void plushError_print_new_error(char *message) {
    Error err = plushError_new_error();
    plushError_set_error_message(err, message);
    plushError_print_error(err);
    plushError_destroy_error(err);
    return;
}

void plushError_print_new_warn(char* message) {
    char* tmp = (char*)calloc(PLUSH_MAX_ERROR_LENGTH, sizeof(char));
    snprintf(tmp, PLUSH_MAX_ERROR_LENGTH, "[%sWarn%s] %s\n", COLOR_YELLOWBOLD, COLOR_BASE, message);
    ssize_t bytes_written = write(STDERR_FILENO, tmp, PLUSH_MAX_ERROR_LENGTH);
    (void)bytes_written;
    return;
}
