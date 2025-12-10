/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:31:40 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:31:41 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/envp.h"
#include "../../../../includes/executor.h"
#include "../../../../includes/minishell.h"
#include "../../../../lib/libft/libft.h"
#include "utils.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int	cd_error(const char *msg, const char *path)
{
	write(2, "minishell: cd: ", 15);
	if (path)
	{
		write(2, path, ft_strlen(path));
		write(2, ": ", 2);
	}
	if (msg)
		write(2, msg, ft_strlen(msg));
	else
		perror(NULL);
	return (1);
}

static int	get_home_path(const t_minishell *sh, char *path)
{
	char	*subst;

	subst = envp_getvar_value(sh->env, "HOME", sh->last_status);
	if (!subst)
		return (cd_error("HOME not set\n", NULL));
	ft_strlcpy(path, subst, ARG_MAX);
	free(subst);
	return (0);
}

static int	get_oldpwd_path(const t_minishell *sh, char *path)
{
	char	*subst;

	subst = envp_getvar_value(sh->env, "OLDPWD", sh->last_status);
	if (!subst)
		return (cd_error("OLDPWD not set\n", NULL));
	ft_strlcpy(path, subst, ARG_MAX);
	write(1, path, ft_strlen(path));
	write(1, "\n", 1);
	free(subst);
	return (0);
}

static int	resolve_path(const t_minishell *sh, char **argv, int argc,
		char *path)
{
	bool	no_op_follow;

	no_op_follow = (argc >= 2) && ft_strcmp(argv[1], "--") == 0;
	if (argc > 2 + no_op_follow)
		return (cd_error("too many arguments\n", NULL));
	if (argc == 1 || (argc == 2 && no_op_follow))
		return (get_home_path(sh, path));
	if (!*argv[1 + no_op_follow])
		ft_strlcpy(path, ".", 2);
	else if (ft_strcmp(argv[1 + no_op_follow], "-") == 0)
		return (get_oldpwd_path(sh, path));
	else
		ft_strlcpy(path, argv[1 + no_op_follow], ARG_MAX);
	return (0);
}

int	exec_cd(const t_minishell *sh, char **argv, const int argc)
{
	char	path[ARG_MAX];
	char	oldpwd[ARG_MAX];
	int		ret;

	ret = resolve_path(sh, argv, argc, path);
	if (ret != 0)
		return (ret);
	getcwd(oldpwd, ARG_MAX);
	if (chdir(path) == -1)
		return (cd_error(NULL, path));
	envp_setvar(sh->env, "OLDPWD", oldpwd, EXPORT);
	envp_setvar(sh->env, "PWD", getcwd(path, ARG_MAX), EXPORT);
	return (0);
}
