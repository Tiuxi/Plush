#include "parser/tokenParser.h"

#define isNumber(c) ( \
    c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || \
    c == '5' || c == '6' || c == '7' || c == '8' || c == '9'    \
)

#define isWord(c) ( \
       c != ' '  && c != ';'  && c != '\n' && c != '\0'         \
    && c != '<'  && c != '>'  && c != '&'  && c != '|'          \
)

bool plushToken_isFile(char* token) {
    if (token == NULL) return FALSE;

    for (int i=0; token[i]!='\0'; i++)
        if (token[i] == '/')
            return TRUE;

    return FALSE;
}

bool plushToken_isStdin(char* token) {
    return (strcmp(token, "<") == 0);
}

bool plushToken_isStdout(char* token) {
    return (strcmp(token, ">") == 0)
        || (strcmp(token, ">>") == 0)
        || (strcmp(token, "1>") == 0)
        || (strcmp(token, "1>>") == 0);
}

bool plushToken_isStderr(char* token) {
    return (strcmp(token, "2>") == 0)
        || (strcmp(token, "2>>") == 0);
}

Plush_Token* plushToken_tokenize(char* _input) {
    int nbTokenAllocated = 8;
    Plush_Token* tokenList = (Plush_Token*)malloc(sizeof(Plush_Token) * nbTokenAllocated);

    if (_input == NULL) {
        tokenList[0].type = Token_END_OF_INPUT;
        tokenList[0].token = (char*)malloc(sizeof(char));
        tokenList[0].token[0] = '\0';
        return tokenList;
    }

    int index = 0, tokenIndex = 0;
    int inputLength = strlen(_input) + 1;
    char* input = (char*)malloc(sizeof(char) * inputLength);
    memcpy(input, _input, inputLength-1);
    input[inputLength-1] = '\0';

    // main loop
    while (index < inputLength) {
        int currentTokenLength = 0;
        int currentTokenAllocated = 32;

        // skip spaces
        while (input[index] == ' ' || input[index] == '\t')
            index++;
        
        // word and redirection with fd indication 
        if (isWord(input[index])) {
            tokenList[tokenIndex].type = Token_WORD;
            tokenList[tokenIndex].token = (char*)malloc(sizeof(char) * currentTokenAllocated);

            // check redirection
            while(isNumber(input[index])) {
                tokenList[tokenIndex].token[currentTokenLength] = input[index];
                index++;
                currentTokenLength++;
                
                if (currentTokenLength == currentTokenAllocated) {
                    currentTokenAllocated *= 2;
                    tokenList[tokenIndex].token = (char*)realloc(tokenList[tokenIndex].token, currentTokenAllocated);
                }
            }
            int redirectionNbChar = 0;
            while (input[index] == '<' || input[index] == '>') {
                tokenList[tokenIndex].type = Token_REDIRECT;
                tokenList[tokenIndex].token[currentTokenLength] = input[index];
                index++;
                currentTokenLength++;
                redirectionNbChar++;

                if (currentTokenLength == currentTokenAllocated) {
                    currentTokenAllocated *= 2;
                    tokenList[tokenIndex].token = (char*)realloc(tokenList[tokenIndex].token, currentTokenAllocated);
                }
                if (redirectionNbChar == 2)
                    break;
            }

            if (tokenList[tokenIndex].type != Token_REDIRECT) {
                while (isWord(input[index])) {
                    tokenList[tokenIndex].token[currentTokenLength] = input[index];
                    index++;
                    currentTokenLength++;

                    if (currentTokenLength == currentTokenAllocated) {
                        currentTokenAllocated *= 2;
                        tokenList[tokenIndex].token = (char*)realloc(tokenList[tokenIndex].token, currentTokenAllocated);
                    }
                }
            }
        }

        // redirection
        else if (input[index] == '<' || input[index] == '>') {
            tokenList[tokenIndex].type = Token_REDIRECT;
            tokenList[tokenIndex].token = (char*)malloc(sizeof(char) * currentTokenAllocated);

            while (input[index] == '<' || input[index] == '>') {
                tokenList[tokenIndex].token[currentTokenLength] = input[index];
                index++;
                currentTokenLength++;

                if (currentTokenLength == 2)
                    break;
            }
        }

        // and
        else if (input[index] == '&') {
            tokenList[tokenIndex].type = Token_AND;
            tokenList[tokenIndex].token = (char*)malloc(sizeof(char) * currentTokenAllocated);

            while (input[index] == '&') {
                tokenList[tokenIndex].token[currentTokenLength] = input[index];
                index++;
                currentTokenLength++;

                if (currentTokenLength == 2)
                    break;
            }
        }

        // or
        else if (input[index] == '|') {
            tokenList[tokenIndex].type = Token_OR;
            tokenList[tokenIndex].token = (char*)malloc(sizeof(char) * currentTokenAllocated);

            while (input[index] == '|') {
                tokenList[tokenIndex].token[currentTokenLength] = input[index];
                index++;
                currentTokenLength++;

                if (currentTokenLength == 2) {
                    tokenList[tokenIndex].token[currentTokenLength] = '\0';
                    break;
                }
            }
        }

        // End-Of-Command
        else if (input[index] == ';' || input[index] == '\n') {
            tokenList[tokenIndex].type = Token_END_OF_COMMAND;
            tokenList[tokenIndex].token = (char*)malloc(sizeof(char) * currentTokenAllocated);
            tokenList[tokenIndex].token[0] = input[index];
            tokenList[tokenIndex].token[1] = '\0';
            index++;
        }

        // End-Of-Input
        else if (input[index] == '\0') {
            tokenList[tokenIndex].type = Token_END_OF_INPUT;
            tokenList[tokenIndex].token = (char*)malloc(sizeof(char) * currentTokenAllocated);
            tokenList[tokenIndex].token[0] = '\0';
            break;
        }

        tokenList[tokenIndex].token[currentTokenLength] = '\0';
        tokenIndex++;
        if (tokenIndex+1 >= nbTokenAllocated) {
            nbTokenAllocated *= 2;
            tokenList = (Plush_Token*)realloc(tokenList, sizeof(Plush_Token) * nbTokenAllocated);
            ASSERT(tokenList != NULL);
        }
    }

    free(input);
    return tokenList;
}

void plushToken_freeTokenList(Plush_Token* list) {
    int i = 0;
    while (list[i].type != Token_END_OF_INPUT) {
        free(list[i].token);
        i++;
    }
    free(list[i].token);
    free(list);
}
