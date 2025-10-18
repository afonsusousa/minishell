//
// Created by afonsusousa on 10/15/25.
//

#ifndef MINISHELL_PIPELINE_H
#define MINISHELL_PIPELINE_H
#include <sys/types.h>

typedef struct s_pipeline
{
    int     pipefd[2];
    int     prev_read;
    pid_t   pids[256];
    size_t  count;
} t_pipeline;

#endif //MINISHELL_PIPELINE_H