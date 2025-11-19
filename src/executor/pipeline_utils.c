//
// Created by afonsusousa on 11/19/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../../includes/sig.h"
#include "../../includes/minishell.h"

int setup_pipe(t_pipeline *pipeline, const bool has_next)
{
    if (has_next && pipe(pipeline->pipefd) < 0)
    {
        perror("pipe");
        if (pipeline->prev_read != -1)
            close(pipeline->prev_read);
        return (1);
    }
    return (0);
}

int setup_child_fds(t_pipeline *pipeline, const bool has_next)
{
    if (pipeline->prev_read != -1)
    {
        if (dup2(pipeline->prev_read, STDIN_FILENO) < 0)
            return (1);
        close(pipeline->prev_read);
    }
    if (has_next)
    {
        close(pipeline->pipefd[0]);
        if (dup2(pipeline->pipefd[1], STDOUT_FILENO) < 0)
            return (1);
        close(pipeline->pipefd[1]);
    }
    return (0);
}

void close_parent_fds(t_pipeline *pipeline, const bool has_next)
{
    if (pipeline->prev_read != -1)
        close(pipeline->prev_read);
    if (has_next)
    {
        close(pipeline->pipefd[1]);
        pipeline->prev_read = pipeline->pipefd[0];
    }
}

int wait_pids(const t_pipeline *pipeline)
{
    size_t i;
    int status;
    int st;

    i = 0;
    status = 0;
    while (i < pipeline->count)
    {
        signal(SIGINT, SIG_IGN);
        if (waitpid(pipeline->pids[i], &st, 0) > 0 && i == pipeline->count - 1)
        {
            if (WIFEXITED(st))
                status = WEXITSTATUS(st);
            else if (WIFSIGNALED(st))
                status = 128 + WTERMSIG(st);
            if (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
                write(1, "\n", 1);
        }
        signal(SIGINT, sigint_handler);
        i++;
    }
    return (status);
}

