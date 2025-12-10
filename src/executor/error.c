//
// Created by afonsusousa on 11/19/25.
//

#include "../../includes/minishell.h"
#include "../../lib/libft/libft.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void	print_open_error(const char *filename)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(filename);
}

void	print_dup2_error(void)
{
	ft_putstr_fd("minishell: dup2: ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}