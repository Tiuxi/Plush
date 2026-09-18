#include "syntaxParser.h"

Plush_Command PlushSyntax_new_command() {
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

void PlushSyntax_destroy_command(Plush_Command command) {
    if (command->command != NULL) free(command->command);
    if (command->stdin_redirect != NULL) free(command->stdin_redirect);
    if (command->stdout_redirect != NULL) free(command->stdout_redirect);
    if (command->stderr_redirect != NULL) free(command->stderr_redirect);

    List lst = command->conditionTrue_command;
    while (lst != NULL) {
        PlushSyntax_destroy_command(lst->v);
        List tmp = lst;
        lst = lst->next;
        free(tmp);
    }

    lst = command->conditionFalse_command;
    while (lst != NULL) {
        PlushSyntax_destroy_command(lst->v);
        List tmp = lst;
        lst = lst->next;
        free(tmp);
    }

    free(command);
}

