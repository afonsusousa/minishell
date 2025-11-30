//
// Created by afonsusousa on 11/4/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../../includes/minishell.h"
#include "../../../includes/executor.h"


static int setup_fds(const t_minishell *sh)
{
    if (sh->pipeline.io[READ_END] != STDIN_FILENO &&
        dup2(sh->pipeline.io[READ_END], STDIN_FILENO) < 0)
            return (perror("dup2"), 1);
    if (sh->pipeline.io[WRITE_END] != STDOUT_FILENO
        && dup2(sh->pipeline.io[WRITE_END], STDOUT_FILENO) < 0)
            return (perror("dup2"), 1);
    return (0);
}

static int fork_core(t_minishell *sh, const t_ast *core)
{
    int     status;
    pid_t   pid;

    pid = fork();
    if (pid < 0)
        return (-1);
    if (pid == 0)
    {
        if (setup_fds(sh) != 0)
            exit(1);
        sh->pipeline.count = 0;
        status = exec_core(sh, core, true);
        minishell_free(sh);
        exit(status);
    }
    sh->pipeline.pids[sh->pipeline.count++] = pid;
    return (0);
}

static int exec_pipeline_core(t_minishell *sh, const t_ast_list *core)
{
    sh->pipeline.io[WRITE_END] = STDOUT_FILENO;
    if (core->next)
    {
        if (pipe(sh->pipeline.pipefd) < 0)
        {
            if (sh->pipeline.io[READ_END] != STDIN_FILENO)
                close(sh->pipeline.io[READ_END]);
            return (perror("pipe"), 1);
        }
        sh->pipeline.io[WRITE_END] = sh->pipeline.pipefd[WRITE_END];
    }
    if (fork_core(sh, core->node) < 0)
        return (pipeline_fork_error(sh, core));
    if (sh->pipeline.io[READ_END] != STDIN_FILENO)
        close(sh->pipeline.io[READ_END]);
    if (core->next)
    {
        close(sh->pipeline.pipefd[WRITE_END]);
        sh->pipeline.io[READ_END] = sh->pipeline.pipefd[READ_END];
    }
    return (0);
}

int exec_pipeline(t_minishell *sh, const t_ast_list *cores)
{
    const t_ast_list    *curr;

    sh->pipeline.count = 0;
    sh->pipeline.io[READ_END] = -1;
    sh->pipeline.io[WRITE_END] = -1;
    if (!cores)
        return (0);
    if (!cores->next && is_core_builtin(cores->node))
        return (exec_core(sh, cores->node, false));
    curr = cores;
    sh->pipeline.io[READ_END] = STDIN_FILENO;
    while (curr && sh->pipeline.count < 128)
    {
        if (exec_pipeline_core(sh, curr) != 0)
            return (1);
        curr = curr->next;
    }
    return (wait_pids((&sh->pipeline)));
}