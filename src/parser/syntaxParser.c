#include "syntaxParser.h"

typedef struct s_commands_node {
    Plush_Token* tokens;
    int tokenIndex;

    Plush_Command* commands;
    int commandIndex;
    int commandAllocated;

    int currentCommandCharAllocated;
    int currentCommandLength;

    bool abortParsing;
} Parser;

void commandList(Parser* parser);
void command(Parser* parser);
void exe(Parser* parser);
void args(Parser* parser);
void redirection(Parser* parser);


/********************************************************/
/******************** Private method ********************/
/********************************************************/

Plush_Token_Type peekTokenType(Parser* parser) {
    return parser->tokens[parser->tokenIndex].type;
}

void addTokenToCommand(Parser* parser) {
    int lenToken = strlen(parser->tokens[parser->tokenIndex].token);
    int lenCommand = parser->currentCommandLength;

    if ((lenCommand + lenToken + 2) >= parser->currentCommandCharAllocated) {
        parser->currentCommandCharAllocated *= 2;
        parser->commands[parser->commandIndex]->command = realloc(
            parser->commands[parser->commandIndex]->command,
            parser->currentCommandCharAllocated
        );
        ASSERT(parser->commands[parser->commandIndex]->command != NULL);
    }

    if (parser->currentCommandLength != 0) {
        parser->commands[parser->commandIndex]->command[parser->currentCommandLength] = ' ';
        parser->currentCommandLength++;
    }

    memcpy(
        parser->commands[parser->commandIndex]->command + parser->currentCommandLength,
        parser->tokens[parser->tokenIndex].token,
        lenToken
    );

    parser->currentCommandLength += lenToken;
    parser->commands[parser->commandIndex]->command[parser->currentCommandLength] = '\0';
}

void setupNextCommand(Parser* parser) {
    parser->commandIndex++;
    if (parser->commandIndex >= parser->commandAllocated) {
        parser->commandAllocated *= 2;
        parser->commands = (Plush_Command*)realloc(parser->commands, sizeof(Plush_Command) * parser->commandAllocated);
        ASSERT(parser->commands != NULL);

        for (int i=parser->commandAllocated/2; i< parser->commandAllocated; i++) 
            parser->commands[i] = NULL;
    }

    parser->currentCommandCharAllocated = 64;
    parser->currentCommandLength = 0;
    parser->commands[parser->commandIndex] = plushSyntax_new_command();
    parser->commands[parser->commandIndex]->command = (char*)malloc(sizeof(char) * parser->currentCommandCharAllocated);
    parser->commands[parser->commandIndex]->command[0] = '\0';
}


/********************************************************/
/******************** Public method *********************/
/********************************************************/

Plush_Command plushSyntax_new_command() {
    Plush_Command command = (Plush_Command)malloc(sizeof(struct s_plushCommand));
    command->command = NULL;
    command->type = COMMAND_BASE;

    command->stdin_redirect = NULL;
    command->stdout_redirect = NULL;
    command->stderr_redirect = NULL;

    command->conditionTrue_command = NULL;
    command->conditionFalse_command = NULL;

    return command;
}

void plushSyntax_destroy_command(Plush_Command command) {
    if (command->command != NULL) free(command->command);
    if (command->stdin_redirect != NULL) free(command->stdin_redirect);
    if (command->stdout_redirect != NULL) free(command->stdout_redirect);
    if (command->stderr_redirect != NULL) free(command->stderr_redirect);

    List lst = command->conditionTrue_command;
    while (lst != NULL) {
        plushSyntax_destroy_command(lst->v);
        List tmp = lst;
        lst = lst->next;
        free(tmp);
    }

    lst = command->conditionFalse_command;
    while (lst != NULL) {
        plushSyntax_destroy_command(lst->v);
        List tmp = lst;
        lst = lst->next;
        free(tmp);
    }

    free(command);
}

// equivalent to "program" rule
Plush_Command* plushSyntax_parseInput(char* input) {
    Parser parser;

    parser.tokenIndex = 0;
    parser.tokens = plushToken_tokenize(input);

    // "program : empty" rule
    if (parser.tokens[0].type == Token_END_OF_INPUT) {
        plushToken_freeTokenList(parser.tokens);
        return NULL;
    }

    parser.commandAllocated = 2;
    parser.commandIndex = 0;
    parser.commands = (Plush_Command*)malloc(sizeof(Plush_Command) * parser.commandAllocated);
    for (int i=0; i < parser.commandAllocated; i++) 
        parser.commands[i] = NULL;

    parser.currentCommandCharAllocated = 64;
    parser.currentCommandLength = 0;
    parser.commands[parser.commandIndex] = plushSyntax_new_command();
    parser.commands[parser.commandIndex]->command = (char*)malloc(sizeof(char) * parser.currentCommandCharAllocated);

    parser.abortParsing = FALSE;

    commandList(&parser);

    plushToken_freeTokenList(parser.tokens);

    if (parser.abortParsing) {
        for (int i=0; parser.commands[i] != NULL; i++) {
            plushSyntax_destroy_command(parser.commands[i]);
        }
        free(parser.commands);

        return NULL;
    }

    if (parser.commands[0] == NULL) {
        free(parser.commands);
        return NULL;
    }

    return parser.commands;
}


/*****************************************************/
/********************   Grammar   ********************/
/*****************************************************/

void commandList(Parser* parser) {
    while ((peekTokenType(parser) != Token_END_OF_INPUT) && !(parser->abortParsing)) {
        if (peekTokenType(parser) == Token_WORD || peekTokenType(parser) == Token_END_OF_COMMAND) {
            command(parser);
        }

        else if (peekTokenType(parser) == Token_AND) {
            if ((parser->commandIndex <= 0) || (parser->commands[parser->commandIndex-1]->type == COMMAND_NULL)) {
                plushError_print_error("Parsing : expected an executable before &&.");
                parser->abortParsing = TRUE;
                return;
            }

            parser->commands[parser->commandIndex-1]->type = COMMAND_AND;
            parser->tokenIndex++;

            if (parser->tokens[parser->tokenIndex].type != Token_WORD) {
                plushError_print_error("Parsing : expected an executable after &&.");
                parser->abortParsing = TRUE;
                return;
            }
        }

        else if (peekTokenType(parser) == Token_OR) {
            if ((parser->commandIndex <= 0) || (parser->commands[parser->commandIndex-1]->type == COMMAND_NULL)) {
                plushError_print_error("Parsing : expected an executable before ||.");
                parser->abortParsing = TRUE;
                return;
            }

            parser->commands[parser->commandIndex-1]->type = COMMAND_OR;
            parser->tokenIndex++;

            if (parser->tokens[parser->tokenIndex].type != Token_WORD) {
                plushError_print_error("Parsing : expected an executable after ||.");
                parser->abortParsing = TRUE;
                return;
            }
        }

        else if (peekTokenType(parser) == Token_REDIRECT) {
            plushError_print_error("Parsing : expected an executable before a redirection.");
            parser->abortParsing = TRUE;
            return;
        }
    }

    if (peekTokenType(parser) == Token_END_OF_INPUT && parser->commands[parser->commandIndex] != NULL) {
        plushSyntax_destroy_command(parser->commands[parser->commandIndex]);
        parser->commands[parser->commandIndex] = NULL;
    }
}

void command(Parser* parser) {
    if (parser->abortParsing) return;

    if (peekTokenType(parser) == Token_WORD) {
        exe(parser);
        if (parser->abortParsing) return;

        while (peekTokenType(parser) == Token_WORD || peekTokenType(parser) == Token_REDIRECT) {
            args(parser);
            if (parser->abortParsing) return;

            redirection(parser);
            if (parser->abortParsing) return;
        }

        if (peekTokenType(parser) == Token_END_OF_COMMAND) {
            parser->tokenIndex++;
        }

        setupNextCommand(parser);
    }

    else if (peekTokenType(parser) == Token_END_OF_COMMAND) {
        parser->tokenIndex++;
        free(parser->commands[parser->commandIndex]->command);
        parser->commands[parser->commandIndex]->command = NULL;
        parser->commands[parser->commandIndex]->type = COMMAND_NULL;

        setupNextCommand(parser);
    }

    else if (peekTokenType(parser) == Token_END_OF_INPUT) {
        if (parser->commands[parser->commandIndex] != NULL)
            plushSyntax_destroy_command(parser->commands[parser->commandIndex]);
        parser->commands[parser->commandIndex] = NULL;
    }

    else {
        plushError_print_error("Parsing : unexpected token %s.", parser->tokens[parser->tokenIndex].token);
    }
}

void exe(Parser* parser) {
    if (parser->abortParsing) return;

    if (peekTokenType(parser) == Token_WORD) {
        addTokenToCommand(parser);
        
        parser->tokenIndex++;
    } else {
        plushError_print_error("Parsing : expected an executable but instead got \"%s\".", 
            parser->tokens[parser->tokenIndex].token
        );
        parser->abortParsing = TRUE;
    }
}

void args(Parser* parser) {
    if (parser->abortParsing) return;

    while (peekTokenType(parser) == Token_WORD) {
        addTokenToCommand(parser);
        parser->tokenIndex++;
    }
}

void redirection(Parser* parser) {
    if (parser->abortParsing) return;

    while ((peekTokenType(parser) == Token_REDIRECT) && !(parser->abortParsing)) {
        char* redirectToken = parser->tokens[parser->tokenIndex].token;
        parser->tokenIndex++;

        if (peekTokenType(parser) != Token_WORD) {
            plushError_print_error("Parsing : expected a file after redirection, but instead got \"%s\".",
                parser->tokens[parser->tokenIndex].token
            );
            parser->abortParsing = TRUE;
        } else {
            int fileNameLength = strlen(parser->tokens[parser->tokenIndex].token) + 1;

            if (plushToken_isStdin(redirectToken)) {
                if (parser->commands[parser->commandIndex]->stdin_redirect != NULL) {
                    plushError_print_warn("stdin is redirected multiple times. Only last redirection will be effective.");
                    free(parser->commands[parser->commandIndex]->stdin_redirect);
                }

                parser->commands[parser->commandIndex]->stdin_redirect = (char*)malloc(sizeof(char) * fileNameLength);
                memcpy(
                    parser->commands[parser->commandIndex]->stdin_redirect,
                    parser->tokens[parser->tokenIndex].token,
                    fileNameLength - 1
                );
                parser->commands[parser->commandIndex]->stdin_redirect[fileNameLength-1] = '\0';
            }
            else if (plushToken_isStdout(redirectToken)) {
                if (parser->commands[parser->commandIndex]->stdout_redirect != NULL) {
                    plushError_print_warn("stdout is redirected multiple times. Only last redirection will be effective.");
                    free(parser->commands[parser->commandIndex]->stdout_redirect);
                }

                parser->commands[parser->commandIndex]->stdout_redirect = (char*)malloc(sizeof(char) * fileNameLength);
                memcpy(
                    parser->commands[parser->commandIndex]->stdout_redirect,
                    parser->tokens[parser->tokenIndex].token,
                    fileNameLength - 1
                );
                parser->commands[parser->commandIndex]->stdout_redirect[fileNameLength-1] = '\0';
            }
            else if (plushToken_isStderr(redirectToken)) {
                if (parser->commands[parser->commandIndex]->stderr_redirect != NULL) {
                    plushError_print_warn("stderr is redirected multiple times. Only last redirection will be effective.");
                    free(parser->commands[parser->commandIndex]->stderr_redirect);
                }

                parser->commands[parser->commandIndex]->stderr_redirect = (char*)malloc(sizeof(char) * fileNameLength);
                memcpy(
                    parser->commands[parser->commandIndex]->stderr_redirect,
                    parser->tokens[parser->tokenIndex].token,
                    fileNameLength - 1
                );
                parser->commands[parser->commandIndex]->stderr_redirect[fileNameLength-1] = '\0';
            } 
            else {
                plushError_print_warn(
                    "Redirection is currently only supported for stdin, stdout and stderr. The redirection \"%s %s\" will be skipped",
                    parser->tokens[parser->tokenIndex-1].token,
                    parser->tokens[parser->tokenIndex].token
                );
            }

            parser->tokenIndex++;
        }
    }
}