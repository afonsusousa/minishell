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

static int setup_pipe(t_pipeline *pipeline, const bool has_next)
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

static int setup_child_fds(t_pipeline *pipeline, const bool has_next)
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

static void execute_pipeline_child(t_minishell *sh, t_pipeline *pipeline,
                                   const t_ast *node, const bool has_next)
{
    int status;

    if (setup_child_fds(pipeline, has_next) != 0)
        exit(1);
    status = exec_core(sh, node, true);
    minishell_free(sh);
    exit(status);
}

static void close_parent_fds(t_pipeline *pipeline, const bool has_next)
{
    if (pipeline->prev_read != -1)
        close(pipeline->prev_read);
    if (has_next)
    {
        close(pipeline->pipefd[1]);
        pipeline->prev_read = pipeline->pipefd[0];
    }
}

static int should_run_builtin_directly(const t_ast_list *cores)
{
    if (!cores->next && cores->node->type == AST_COMMAND)
    {
        if (!cores->node->as.command.argv
            || is_builtin((char *)cores->node->as.command.argv[0]))
            return (1);
    }
    return (0);
}

static int execute_pipeline_loop(t_minishell *sh, const t_ast_list *cores)
{
    t_pipeline *pipeline;

    pipeline = &sh->pipeline;
    while (cores)
    {
        if (setup_pipe(pipeline, cores->next != NULL) != 0)
            return (1);
        pipeline->pids[pipeline->count] = fork();
        if (pipeline->pids[pipeline->count] < 0)
            return (handle_fork_error(pipeline, cores->next != NULL));
        if (pipeline->pids[pipeline->count] == 0)
            execute_pipeline_child(sh, pipeline, cores->node, cores->next != NULL);
        close_parent_fds(pipeline, cores->next != NULL);
        pipeline->count++;
        cores = cores->next;
    }
    return (0);
}

int exec_pipeline(t_minishell* sh, const t_ast_list* cores)
{
    int         status;
    t_pipeline  *pipeline;

    pipeline = &sh->pipeline;
    memset(pipeline, 0, sizeof(t_pipeline));
    pipeline->prev_read = -1;
    if (should_run_builtin_directly(cores))
        return (exec_command(sh, cores->node, false));
    if (execute_pipeline_loop(sh, cores) != 0)
        return (1);
    if (pipeline->prev_read != -1)
        close(pipeline->prev_read);
    status = wait_pids(pipeline);
    sh->last_status = status;
    return (status);
}

