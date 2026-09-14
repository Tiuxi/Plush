#ifndef PLUSH_ERROR
#define PLUSH_ERROR

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "utils/constants.h"
#include "utils/color.h"

/**
 * Print the error message on the out stream STDOUT
 * 
 * @param err The error to print
 */
void plushError_print_error(const char* error, ...);

/**
 * Print a warn message directly from a string
 * 
 * @param message The message to put in the warn
 */
void plushError_print_warn(const char* warn, ...);

#endif