/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:32:14 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:39:05 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/envp.h"
#include "../../../../includes/executor.h"
#include "../../../../includes/minishell.h"
#include "../../../../includes/utils.h"
#include "../../../../lib/libft/libft.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static int	exec_assignments(t_minishell *sh, const char **a, bool context)
{
	t_envp	*env;
	int		flags;

	if (context)
		env = sh->ctx;
	else
		env = sh->env;
	flags = EXPORT | EXPAND_VARS | CONSUME_QUOTES;
	while (a && *a)
		if (envp_setvar_str(env, *a++, sh->last_status, flags) == NULL)
			return (1);
	return (0);
}

static void	exit_wrapper(char ***argv, char *orig_cmd, int exit_code)
{
	free_argv(*argv);
	free(orig_cmd);
	exit(exit_code);
}

static void	execve_error(char ***argv, char *orig_cmd)
{
	struct stat	path_stat;
	char		*cmd;

	cmd = **argv;
	write(2, "minishell: ", 11);
	if (*cmd)
		write(2, cmd, ft_strlen(cmd));
	else
		write(2, "''", 2);
	write(2, ": ", 2);
	if (!ft_strchr(cmd, '/'))
		exit_wrapper(argv, orig_cmd, (write(2, "command not found\n",
					18) & 0) | 127);
	if (stat(cmd, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		exit_wrapper(argv, orig_cmd, (write(2, "Is a directory\n",
					15) & 0) | 126);
	if (errno == EACCES)
		exit_wrapper(argv, orig_cmd, (write(2, "Permission denied\n",
					18) & 0) | 126);
	if (errno == ENOENT)
		exit_wrapper(argv, orig_cmd, (write(2, "No such file or directory\n",
					26) & 0) | 127);
	perror("");
	exit_wrapper(argv, orig_cmd, 126);
}

int	execve_wrapper(t_minishell *sh, char ***argv)
{
	char	**env_arr;
	char	*orig_cmd;

	if (!argv || !*argv || !**argv)
		return (0);
	orig_cmd = ft_strdup(**argv);
	**argv = find_path(sh, **argv);
	envp_setvar(sh->env, "_", **argv, EXPORT);
	env_arr = get_envp_array(sh->env, true);
	minishell_free(sh);
	execve(**argv, *argv, env_arr);
	free_envp(sh->ctx);
	free_until_null(&env_arr);
	execve_error(argv, orig_cmd);
	return (126);
}

int	exec_command(t_minishell *sh, const t_ast *core)
{
	char	**argv;
	int		status;

	memset(sh->ctx, 0, sizeof(t_envp));
	if (!core || core->type != AST_COMMAND)
		return (1);
	argv = argv_to_arr(sh, core->u_as.s_command.argv,
			(int *)&core->u_as.s_command.argc);
	if (exec_redirs(sh, core->u_as.s_command.redirs))
		return (free_argv(argv), 1);
	if (exec_assignments(sh, core->u_as.s_command.assignments, argv != NULL))
		return (free_argv(argv), 1);
	if (!argv)
		return (0);
	if (is_builtin(argv[0]))
	{
		status = exec_builtin(sh, argv, core->u_as.s_command.argc);
		free_argv(argv);
		sh->last_status = status;
		return (status);
	}
	status = execve_wrapper(sh, &argv);
	sh->last_status = status;
	return (status);
}
