//
// Created by afonsusousa on 10/17/25.
//
#include <unistd.h>
#include <sys/wait.h>

#include "../../includes/executor.h"
#include "../../includes/minishell.h"

int exec_pipeline(t_minishell *sh, t_ast_list *cmds)
{
    int fd[2];
    pid_t pid;
    int status;
    int previous_read;

    previous_read = -1;
    while (cmds)
    {
        pipe(fd);
        pid = fork();
        if (pid == 0)
        {
            if (previous_read != -1)
            {
                close(fd[1]);
                dup2(previous_read, STDIN_FILENO);
            }
            if (cmds->next)
            {
                close(fd[0]);
                dup2(fd[1],STDOUT_FILENO);
            }
            exec_command(sh, cmds->node, true);
        }
        previous_read = fd[0];
    }
    waitpid(pid, &status, 0);
    return(status);
}