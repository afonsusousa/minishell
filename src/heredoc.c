//
// Created by afonsusousa on 10/14/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include "../includes/subst.h"
#include <readline/history.h>
#include <sys/wait.h>

int heredoc_fd(const char *delimiter)
{
    pid_t   pid;
    char	*line;
    int     fd[2];

    pipe(fd);
    pid = fork();
    if (pid == 0)
    {
        close(fd[0]);
        line = readline("heredoc>");
        while (strcmp(line, delimiter) != 0)
        {
            write(fd[1], line, ft_strlen(line));
            write(fd[1], "\n", 1);
            line = readline("heredoc>");
        }
        exit (0);
    }
    waitpid(pid, NULL, 0);
    close(fd[1]);
    return (fd[0]);
}
