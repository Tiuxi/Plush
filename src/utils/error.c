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
    char* tmp = (char*)malloc(sizeof(char)*PLUSH_MAX_ERROR_LENGTH);
    snprintf(tmp, PLUSH_MAX_ERROR_LENGTH, "[%sError%s] %s\n", COLOR_REDBOLD, COLOR_BASE, message);
    *err = tmp;
}

void plushError_set_error_with_argument(Error err, const char* message, char* arg) {
    char* tmp = (char*)malloc(sizeof(char)*PLUSH_MAX_ERROR_LENGTH);
    snprintf(tmp, PLUSH_MAX_ERROR_LENGTH, "[%sError%s] %s : \"%s\"\n", COLOR_REDBOLD, COLOR_BASE, message, arg);
    *err = tmp;
}

void plushError_print_error(Error err) {
    fprintf(stderr, "%s", *err);
}

void plushError_print_new_error(char *message) {
    Error err = plushError_new_error();
    plushError_set_error_message(err, message);
    plushError_print_error(err);
    plushError_destroy_error(err);
    return;
}
