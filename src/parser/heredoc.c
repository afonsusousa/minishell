//
// Created by afonsusousa on 10/14/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include "../../includes/globbing.h"
#include "../../lib/libft/libft.h"
#include <sys/wait.h>

#include "executor.h"
#include "minishell.h"
static int is_quoted(const char *delimiter)
{
    while (*delimiter)
    {
        if (*delimiter == '\'' || *delimiter == '"')
            return (1);
        delimiter++;
    }
    return (0);
}

void heredoc_setup(const char *delimiter, int heredoc[2])
{
    char	*line;
    int     fd[2];
    char    *del;

    del = (char *) delimiter;
    heredoc[1] = is_quoted(delimiter);
    if (heredoc[1])
        del = expanded(NULL, delimiter, false);
    if (pipe(fd) == 0)
    {
        line = readline("heredoc>");
        while (strcmp(line, del) != 0)
        {
            write(fd[1], line, ft_strlen(line));
            write(fd[1], "\n", 1);
            free(line);
            line = readline("heredoc>");
        }
        if (heredoc[1])
            free(del);
    }
    close(fd[1]);
    heredoc[0] = fd[0];
}