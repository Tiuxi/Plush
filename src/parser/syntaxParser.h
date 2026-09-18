#ifndef PLUSH_SYNTAXE_PARSER
#define PLUSH_SYNTAXE_PARSER

#include <string.h>
#include "utils/error.h"
#include "utils/list.h"

/*

program := commands | empty

commandlist := command
        | command && mandatoryCommand
        | command || mandatoryCommand
        | command ; commandlist
        | command \n commandlist

mandatoryCommand := exe args redirect ; commandlist
        | exe args redirect \n commandlist

command := exe args redirect
        | if (condition) { commandlist }
        | empty

exe := word

args := word args | empty

redirect := < file redirect
        | < file redirect
        | > file redirect
        | 1> file redirect
        | 2> file redirect
        | >> file redirect
        | 2>> file redirect
        | 1>> file redirect
        | empty

file := word

condition := $(exe args)
*/

typedef enum {
    COMMAND_BASE = 0,
    COMMAND_PIPE,
    COMMAND_IF,
    COMMAND_AND,
    COMMAND_OR,

}Plush_Command_Type;

typedef struct s_plushCommand {
    char* command;
    Plush_Command_Type type;

    char* stdin_redirect;
    char* stdout_redirect;
    char* stderr_redirect;

    List conditionTrue_command;
    List conditionFalse_command;
}*Plush_Command;

Plush_Command PlushSyntax_new_command();

void PlushSyntax_destroy_command(Plush_Command command);

#endif /* PLUSH_PARSER */