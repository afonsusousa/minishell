//
// Created by afonsusousa on 11/19/25.
//

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../../lib/libft/libft.h"
#include "../../includes/minishell.h"

void print_open_error(const char *filename)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    perror(filename);
}

void print_dup2_error(void)
{
    ft_putstr_fd("minishell: dup2: ", STDERR_FILENO);
    ft_putstr_fd(strerror(errno), STDERR_FILENO);
    ft_putstr_fd("\n", STDERR_FILENO);
}

int handle_fork_error(const t_pipeline *pipeline, const bool has_next)
{
    perror("fork");
    if (has_next)
    {
        close(pipeline->pipefd[0]);
        close(pipeline->pipefd[1]);
    }
    if (pipeline->prev_read != -1)
        close(pipeline->prev_read);
    return (1);
}