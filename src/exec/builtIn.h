#ifndef PLUSH_BUILTINCOMMAND
#define PLUSH_BUILTINCOMMAND

#include "utils/list.h"
#include "utils/constants.h"
#include "utils/error.h"
#include "hist/history.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int plushBuiltin_check_builtin(List cmd);

#endif