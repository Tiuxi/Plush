#ifndef ROOTSH_ERROR
#define ROOTSH_ERROR

#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "color.h"

typedef char** Error;

/**
 * Create a new error
 * 
 * @return An error with a empty message
 */
Error rootshError_new_error();

/**
 * Free an error
 * 
 * @param err   The error to free
 */
void rootshError_destroy_error(Error err);

/**
 * Set the message of an error
 *
 * @param err The error to put the message in
 * @param message The message to put in the error
 */
void rootshError_set_error_message(Error err, const char *message);

/**
 * Set the message of an error with an argument. 
 * The message will be a concatenated string in the form :
 * `[Error] <message> : <argument>`
 * 
 * @param err The error to put the message in
 * @param message The message to put in the error
 * @param arg The argument to put at the end of a message
 */
void rootshError_set_error_with_argument(Error err, const char *message, char *arg);

/**
 * Print the error message on the out stream STDOUT
 * 
 * @param err The error to print
 */
void rootshError_print_error(Error err);

/**
 * Print an error message directly from a string
 * 
 * @param message The message to put on the error
 */
void rootshError_print_new_error(char* message);

#endif