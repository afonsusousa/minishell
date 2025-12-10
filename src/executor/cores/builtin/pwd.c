/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:32:04 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:32:05 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/executor.h"
#include "../../../../includes/minishell.h"
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>

int	exec_pwd(const t_minishell *sh, char **argv, const int argc)
{
	char	pwd[ARG_MAX];

	(void)sh;
	(void)argv;
	(void)argc;
	getcwd(pwd, ARG_MAX);
	printf("%s\n", pwd);
	return (0);
}
