//
// Created by afonsusousa on 11/4/25.
//

#include <stdbool.h>
#include "../../../../includes/utils.h"
#include "../../../../includes/executor.h"

bool is_builtin (const char *word)
{
    if (!word)
        return (false);
    return (ft_strcmp("export", word) == 0
        || ft_strcmp("unset", word) == 0
        || ft_strcmp("cd", word) == 0
        || ft_strcmp("echo", word) == 0
        || ft_strcmp("env", word) == 0
        || ft_strcmp("pwd", word) == 0
        || ft_strcmp("exit", word) == 0
        );
}