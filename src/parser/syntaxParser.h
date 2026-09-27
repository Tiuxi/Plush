#ifndef PLUSH_SYNTAXE_PARSER
#define PLUSH_SYNTAXE_PARSER

#include <string.h>
#include "utils/error.h"
#include "utils/list.h"

/*

The grammar of the shell.
When a rule can be empty, the next possibles tokens are indicated after, between brackets

tokens : [word, redirectToken, EOC (end-of-command), EOI (end-of-input), &&, ||]


program := commandList EOI | empty [EOI]

(with a "allow empty" flag)
commandList := command
        | mandatoryCommand ( ["&&" | "||"] commandList(false) )*          // left associativity
        | command EOC commandList

mandatoryCommand := exe args redirection

command := exe args redirection
        | if "(" condition ")" "{" commandList "}"
        | empty [EOC, EOI]

exe := word

args := word args
        | empty [<, <<, >, >>, 1>, 2>, 1>>, 2>>, &&, ||, EOC, EOI]

redirection := redirectToken word redirection
        | empty [&&, ||, EOC, EOI]

condition := $(exe args)        // return code 0 = True, return code 1 = False
        | ! condition           // logical not

        // (envvar will be defined later, this rule isn't implemented for now)
        | envvar                // "" = False, anything else = True
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