//
// Created by afonsusousa on 10/14/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include "../includes/subst.h"
#include <sys/wait.h>

//TODO: will need to keep track of pids to then kill in case of Ctrl-C
int heredoc_fd(t_minishell *sh, const char *delimiter)
{
    char	*line;
    int     fd[2];

    pipe(fd);
    sh->heredoc.pids[sh->heredoc.count] = fork();
    // delimiter can be expandable
    if (sh->heredoc.pids[sh->heredoc.count] == 0)
    {
        close(fd[0]);
        line = readline("heredoc>");
        while (strcmp(line, delimiter) != 0)
        {
            write(fd[1], line, ft_strlen(line));
            write(fd[1], "\n", 1);
            free(line);
            line = readline("heredoc>");
        }
        exit (0);
    }
    waitpid(sh->heredoc.pids[sh->heredoc.count++], NULL, 0);
    close(fd[1]);
    return (fd[0]);
}