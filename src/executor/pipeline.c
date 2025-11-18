//
// Created by afonsusousa on 11/4/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "sig.h"
#include "../../includes/minishell.h"
#include "../../includes/executor.h"

static inline int wait_pids(const t_pipeline *pipeline)
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

int exec_pipeline(t_minishell* sh, const t_ast_list* cores)
{
    int status;
    t_pipeline *pipeline;

    pipeline = &sh->pipeline;
    memset(pipeline, 0, sizeof(t_pipeline));
    pipeline->prev_read = -1;
    if (!cores->next)
    {
        if (cores->node->type == AST_COMMAND
            && (!cores->node->as.command.argv
                || is_builtin((char *)cores->node->as.command.argv[0])))
            return (exec_command(sh, cores->node, false));
    }
    while (cores)
    {
        if (cores->next && pipe(pipeline->pipefd) < 0)
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
            if (cores->next)
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
            if (cores->next)
            {
                close(pipeline->pipefd[0]);
                if (dup2(pipeline->pipefd[1], STDOUT_FILENO) < 0)
                    exit(1);
                close(pipeline->pipefd[1]);
            }
            status = exec_core(sh, cores->node, true);
            minishell_free(sh);
            exit(status);
        }
        if (pipeline->prev_read != -1)
            close(pipeline->prev_read);
        if (cores->next)
        {
            close(pipeline->pipefd[1]);
            pipeline->prev_read = pipeline->pipefd[0];
        }
        pipeline->count++;
        cores = cores->next;
    }
    if (pipeline->prev_read != -1)
        close(pipeline->prev_read);
    status = wait_pids(pipeline);
    sh->last_status = status;
    return (status);
}

