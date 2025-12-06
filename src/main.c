#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/history.h>
#include "../includes/sig.h"
#include "../includes/tokens.h"
#include "../includes/parser.h"
#include "../includes/envp.h"
#include "../includes/executor.h"
#include "../lib/libft/libft.h"

static void	minishell_init(t_minishell *sh, t_token_stream *ts,
		t_envp *env, t_envp *ctx)
{
	t_var	*shlvl;

	signal_setup();
	ft_memset(ts, 0, sizeof(t_token_stream));
	ft_memset(env, 0, sizeof(t_envp));
	ft_memset(ctx, 0, sizeof(t_envp));
	ft_memset(sh, 0, sizeof(t_minishell));
	sh->ts = ts;
	sh->env = env;
	sh->ctx = ctx;
	shlvl = envp_getvar(sh->env, "SHLVL");
	if (shlvl && shlvl->value)
		envp_setvar(sh->env, "SHLVL", ft_itoa(ft_atoi(shlvl->value) + 1), EXPORT);
	else
		envp_setvar(sh->env, "SHLVL", ft_strdup("1"), EXPORT);
}

static void	init_env(t_minishell *sh, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
		envp_setvar_str(sh->env, envp[i++], sh->last_status, EXPORT);
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
