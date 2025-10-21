//
// Created by afonsusousa on 10/17/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../../includes/executor.h"
#include "../../includes/minishell.h"


int exec_pipeline(t_minishell* sh, const t_ast_list* cmds)
{
    int status;
    t_pipeline *pipeline;

    pipeline = &sh->pipeline;
    memset(pipeline, 0, sizeof(t_pipeline));
    pipeline->prev_read = -1;
    if (!cmds->next)
    {
        if (!cmds->node->as.simple_command.words)
            return (exec_simple_command(sh, cmds->node, false));
    }
    while (cmds)
    {
        if (cmds->next && pipe(pipeline->pipefd) < 0)
        {
            perror("pipe");
            if (pipeline->prev_read != -1)
                close(pipeline->prev_read);
            return (1);
        }
        pipeline->pids[pipeline->count] = fork();
        if (pipeline->pids[pipeline->count] < 0)
        {
            perror("fork");
            if (cmds->next)
            {
                close(pipeline->pipefd[0]);
                close(pipeline->pipefd[1]);
            }
            if (pipeline->prev_read != -1)
                close(pipeline->prev_read);
            return (1);
        }
        if (pipeline->pids[pipeline->count] == 0)
        {
            if (pipeline->prev_read != -1)
            {
                if (dup2(pipeline->prev_read, STDIN_FILENO) < 0)
                    exit(1);
                close(pipeline->prev_read);
            }
            if (cmds->next)
            {
                close(pipeline->pipefd[0]);
                if (dup2(pipeline->pipefd[1], STDOUT_FILENO) < 0)
                    exit(1);
                close(pipeline->pipefd[1]);
            }
            status = exec_command(sh, cmds->node, true);
            minishell_free(sh);
            exit(status);
        }
        if (pipeline->prev_read != -1)
            close(pipeline->prev_read);
        if (cmds->next)
        {
            close(pipeline->pipefd[1]);
            pipeline->prev_read = pipeline->pipefd[0];
        }
        pipeline->count++;
        cmds = cmds->next;
    }
    if (pipeline->prev_read != -1)
        close(pipeline->prev_read);
    status = wait_pids(pipeline);
    sh->last_status = status;
    return (status);
}