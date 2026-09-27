#ifndef PLUSH_PARSEINPUT
#define PLUSH_PARSEINPUT

#include <string.h>
#include <stdlib.h>
#include "utils/list.h"
#include "utils/constants.h"
#include "utils/error.h"

typedef enum e_token_type {
    Token_WORD,
    Token_REDIRECT,
    Token_END_OF_COMMAND,
    Token_END_OF_INPUT,
    Token_AND,
    Token_OR
} Plush_Token_Type;

typedef struct s_parsed_token {
    Plush_Token_Type type;
    char* token;
} Plush_Token;

/**
 * Return a list of Plush_Token struct and store the number of total tokens in nbTokens.
 * The list is allocated using the malloc function
 *
 * @param input     The string to tokenize
 * @param nbTokens  The variable in which the number of tokens will be stored
 *
 * @return A malloced list of Plush_Token struct
 */
Plush_Token* plushToken_tokenize(char* input, int* nbTokens);


bool plushToken_isFile(char* token);
bool plushToken_isStdin(char* token);
bool plushToken_isStdout(char* token);
bool plushToken_isStderr(char* token);

#endif