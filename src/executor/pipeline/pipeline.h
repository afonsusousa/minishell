//
// Created by afonsusousa on 11/19/25.
//

#ifndef MINISHELL_PIPELINE_UTILS_H
#define MINISHELL_PIPELINE_UTILS_H

#include "../../../includes/minishell.h"

int     setup_pipe(t_pipeline *pipeline, bool has_next);
int     setup_child_fds(const t_pipeline *pipeline, bool has_next);
void    close_parent_fds(t_pipeline *pipeline, bool has_next);
int     wait_pids(const t_pipeline *pipeline);

#endif // MINISHELL_PIPELINE_UTILS_H

