#include "../../../../includes/minishell.h"
#include "../../../../includes/executor.h"
#include "../../../../includes/utils.h"
#include <stdio.h>
//tira +
// char    *sanitize_assignment(const char *str)
// {
//     size_t  i;
//     size_t  size;
//     char    *ret;
//
//     i = 0;
//     while (str[i])
//     {
//         if (str[i] == '+' && str[i + 1] && str[i + 1] == '=')
//             break;
//         i++;
//     }
//     if (!str[i])
//         return (ft_strdup(str));
//     size = ft_strlen(str);
//     ret = calloc(size + 1, sizeof(char));
//     if (!ret)
//         return (NULL);
//     ft_strlcpy(ret, str, i + 1);
//     ft_strlcpy(ret + i, str + i + 1, size - i);
//     return (ret);
// }
//
int print_exported_variables(const t_minishell *sh)
{
    t_var *var;

    var = sh->env->head;
    while (var)
    {
        if (var->value && var->export)
            printf("declare -x %s=\"%s\"\n", var->name, var->value);
        else if (var->export)
            printf("declare -x %s\n", var->name);
        var = var->next;
    }
    return (0);
}

bool is_append(const char *str)
{
    size_t i;

    i = 0;
    while (str && str[i++])
    {
        if (str[i] == '+' && str[i + 1] == '=' && is_valid(str[i - 1]))
            return (true);
        if (!is_valid(str[i]) && str[i] != '+')
            break;
    }
    return (false);
}

 int export(const t_minishell *sh, char **argv)
 {
    t_var *var;
     if (argv[1] == NULL)
         return (print_exported_variables(sh));
     var = NULL;
     while (*argv != NULL)
     {
         argv++;
         if (is_append(*argv))
            var = envp_append_var(sh->env, *argv, true);
         else
            var = envp_setvar(sh->env, *argv, true);
     }
     if (!var)
         return (127);
     return (0);
 }

int unset(t_minishell *sh, char **argv)
{
        if (argv[1] == NULL)
            return (0);
        while (*++argv != NULL)
            envp_unsetvar(sh->env, *argv);
    return (0);
}

 int exec_builtin(t_minishell *sh, char **argv)
 {
   if (ft_strcmp("export", argv[0]) == 0)
         return (export(sh, argv));
     if (ft_strcmp("unset", argv[0]) == 0)
         return (unset(sh, argv));
     return (1);
 }