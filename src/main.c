/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:38:04 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:38:05 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/envp.h"
#include "../includes/executor.h"
#include "../includes/parser.h"
#include "../includes/sig.h"
#include "../includes/tokens.h"
#include "../lib/libft/libft.h"
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void	minishell_init(t_minishell *sh, t_token_stream *ts, t_envp *env,
		t_envp *ctx)
{
	signal_setup();
	ft_memset(ts, 0, sizeof(t_token_stream));
	ft_memset(env, 0, sizeof(t_envp));
	ft_memset(ctx, 0, sizeof(t_envp));
	ft_memset(sh, 0, sizeof(t_minishell));
	sh->ts = ts;
	sh->env = env;
	sh->ctx = ctx;
}

static void	init_env(t_minishell *sh, char **envp)
{
	int		i;
	t_var	*shlvl;
	char	cwd[4096];

	i = 0;
	while (envp[i] != NULL)
		envp_setvar_str(sh->env, envp[i++], sh->last_status, EXPORT);
	shlvl = envp_getvar(sh->env, "SHLVL");
	if (shlvl && shlvl->value)
		envp_setvar(sh->env, "SHLVL", ft_itoa(ft_atoi(shlvl->value) + 1),
			EXPORT);
	else
		envp_setvar(sh->env, "SHLVL", ft_strdup("1"), EXPORT);
	if (getcwd(cwd, sizeof(cwd)))
	{
		envp_setvar(sh->env, "PWD", cwd, EXPORT);
		envp_setvar(sh->env, "OLDPWD", cwd, EXPORT);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell		sh;
	t_token_stream	ts;
	t_envp			env;
	t_envp			ctx;

	minishell_init(&sh, &ts, &env, &ctx);
	init_env(&sh, envp);
	if (argc > 1)
	{
		sh.line = ft_strdup(argv[1]);
		exec_line(&sh);
		minishell_free(&sh);
		return (sh.last_status);
	}
	if (!isatty(STDIN_FILENO))
	{
		while (notty_line(&sh))
		{
			if (sh.line && *sh.line != '\0')
				exec_line(&sh);
			sh.line = NULL;
		}
		minishell_free(&sh);
		return (sh.last_status);
	}
	return (rl_loop(&sh), sh.last_status);
}
