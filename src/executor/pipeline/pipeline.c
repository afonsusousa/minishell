//
// Created by afonsusousa on 11/4/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../includes/minishell.h"
#include "../../../includes/executor.h"
#include "pipeline.h"

static bool is_core_builtin(const t_ast *core)
{
    if (!core)
        return (false);
    if (core->type == AST_COMMAND && core->as.command.argv
        && core->as.command.argv[0])
        return (is_builtin(core->as.command.argv[0]));
    return (false);
}

static void exec_pipeline_child(t_minishell *sh, const t_pipeline *pipeline,
                                   const t_ast *node, const bool has_next)
{
    int status;

    if (setup_child_fds(pipeline, has_next) != 0)
        exit(1);
    status = exec_core(sh, node, true);
    minishell_free(sh);
    exit(status);
}

static int exec_pipeline_loop(t_minishell *sh, const t_ast_list *cores)
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
            exec_pipeline_child(sh, pipeline, cores->node, cores->next != NULL);
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

    if (!cores)
        return (0);
    if (!cores->next && is_core_builtin(cores->node))
        return (exec_core(sh, cores->node, false));
    pipeline = &sh->pipeline;
    memset(pipeline, 0, sizeof(t_pipeline));
    pipeline->prev_read = -1;
    if (exec_pipeline_loop(sh, cores) != 0)
        return (1);
    if (pipeline->prev_read != -1)
        close(pipeline->prev_read);
    status = wait_pids(pipeline);
    sh->last_status = status;
    return (status);
}

