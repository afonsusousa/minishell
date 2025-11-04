//
// Created by afonsusousa on 10/18/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../../../../..//includes/minishell.h"
#include "../../../../..//includes/executor.h"
#include "../../../../..//includes/utils.h"
#include "../../../../../lib/libft/libft.h"

static size_t words_count(t_minishell* sh, const char **w)
{
    size_t c;
    (void) sh;

    c = 0;
    while (w && *w++)
        c++;
    return (c);
}

//TODO: expand wildcards here
char** words_to_argv(t_minishell* sh, const  char **words)
{
    char** argv;
    size_t count;
    size_t i;

    count = words_count(sh, words);
    if (!count)
        return (NULL);
    argv = (char**)calloc(count + 1, sizeof(char*));
    if (!argv)
        return (NULL);
    i = 0;
    while (words && *words)
        argv[i++] = ft_strdup(*words++);
    argv[i] = NULL;
    return (argv);
}

void free_argv(char** argv)
{
    free(argv);
}

//TODO: local access (./)
char* find_path(char* cmd, char** envp)
{
    size_t i;
    char** split_path;
    char* try;

    i = 0;
    while (*envp && (ft_strncmp("PATH=", *envp, 5)))
        envp++;
    if (!*envp)
        return (NULL);
    split_path = ft_split(*envp + 5, ':');
    if (!split_path)
        return (NULL);
    while (split_path[i])
    {
        try = strjoin_three(split_path[i++], "/", cmd);
        if (!try)
            return (free_until_null(&split_path), NULL);
        if (access(try, X_OK) == 0)
            return (free_until_null(&split_path), try);
        free(try);
    }
    return (free_until_null(&split_path), ft_strdup(cmd));
}

int execve_wrapper(t_minishell* sh, char** argv)
{
    char** env_arr;

    if (!argv || !argv[0])
        return (0);
    env_arr = get_envp_array(sh->env);
    env_arr = strjoinjoin(env_arr, get_envp_array(sh->ctx));
    if (is_builtin(argv[0]))
        return (exec_builtin(sh, argv));
    argv[0] = find_path(argv[0], env_arr);
    minishell_free(sh);
    execve(argv[0], argv, env_arr);
    perror("execve");
    exit(127);
}