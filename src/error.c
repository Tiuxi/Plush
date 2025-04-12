#include "error.h"

Error rootshError_new_error() {
    Error err = (Error)malloc(sizeof(char*));
    return err;
}

void rootshError_destroy_error(Error err) {
    if (*err != NULL)
        free(*err);
    
    free(err);
}

void rootshError_set_error_message(Error err, const char *message) {
    char* tmp = (char*)malloc(sizeof(char)*ROOTSH_MAX_ERROR_LENGTH);
    snprintf(tmp, ROOTSH_MAX_ERROR_LENGTH, "[%sError%s] %s\n", COLOR_REDBOLD, COLOR_BASE, message);
    *err = tmp;
}

void rootshError_set_error_with_argument(Error err, const char* message, char* arg) {
    char* tmp = (char*)malloc(sizeof(char)*ROOTSH_MAX_ERROR_LENGTH);
    snprintf(tmp, ROOTSH_MAX_ERROR_LENGTH, "[%sError%s] %s : \"%s\"\n", COLOR_REDBOLD, COLOR_BASE, message, arg);
    *err = tmp;
}

void rootshError_print_error(Error err) {
    printf("%s", *err);
}

void rootshError_print_new_error(char *message) {
    Error err = rootshError_new_error();
    rootshError_set_error_message(err, message);
    rootshError_print_error(err);
    rootshError_destroy_error(err);
    return;
}
