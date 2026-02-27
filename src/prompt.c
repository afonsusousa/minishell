/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:38:09 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 21:30:28 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/executor.h"
#include "../includes/minishell.h"
#include "../includes/parser.h"
#include "../includes/sig.h"
#include "../lib/libft/libft.h"
#include <linux/limits.h>
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

int	exec_line(t_minishell *sh)
{
	if (g_signal_status == SIGINT)
	{
		sh->last_status = 130;
		g_signal_status = 0;
	}
	sh->aborted_parse = false;
	token_stream_init(sh);
	parse(sh);
	token_stream_free(sh->ts);
	if (sh->aborted_parse || !sh->ast)
		return (sh->last_status);
	exec_ast(sh);
	ast_free(sh->ast);
	sh->ast = NULL;
	return (sh->last_status);
}

static void	build_prompt(const char *buffer, const char *user,
		const char *hostname)
{
	ft_strlcat((char *)buffer, (char *)user, 1024);
	ft_strlcat((char *)buffer, "@", 1024);
	ft_strlcat((char *)buffer, (char *)hostname + 6,
		ft_strlen((char *)buffer) + 7);
	ft_strlcat((char *)buffer, ":", 1024);
}

// prompt for school machines
static char	*get_prompt(t_minishell *sh)
{
	char		*home;
	const char	*user;
	char		*hostname;
	char		cwd[1024];
	char		buffer[1024];

	home = envp_getvar_value(sh->env, "HOME", sh->last_status);
	user = getenv("USER");
	hostname = getenv("SESSION_MANAGER");
	if (!user || !(hostname))
		return (free(home), ft_strdup("minishell> "));
	ft_bzero(buffer, 1024);
	build_prompt(buffer, user, hostname);
	getcwd(cwd, 1024);
	if (home && ft_strnstr(cwd, home, ft_strlen(home)) == cwd)
	{
		ft_strlcat(buffer, "~", 1024);
		ft_strlcat(buffer, cwd + ft_strlen(home), 1024);
	}
	else
		ft_strlcat(buffer, cwd, 1024);
	ft_strlcat(buffer, "$ ", 1024);
	return (free(home), ft_strdup(buffer));
}

int	rl_loop(t_minishell *sh)
{
	signal_setup();
	while (1)
	{
		sh->prompt = get_prompt(sh);
		sh->line = readline(sh->prompt);
		if (sh->line == NULL)
			break ;
		if (*sh->line != '\0')
			exec_line(sh);
		if (sh->line)
			add_history(sh->line);
		free(sh->prompt);
		sh->prompt = NULL;
		free(sh->line);
		sh->line = NULL;
	}
	minishell_free(sh);
	return (true);
}

int	notty_line(t_minishell *sh)
{
	ssize_t	rd;
	char	c;
	char	*cursor;
	char	*line;

	line = ft_calloc(ARG_MAX, sizeof(char));
	if (!line)
		return (0);
	cursor = line;
	while (true)
	{
		rd = read(0, &c, 1);
		if (rd < 0)
			return (free(line), perror("read:"), 0);
		if (rd == 0 || c == '\n')
		{
			if (cursor == line && rd == 0)
				return (free(line), 0);
			*cursor = '\0';
			sh->line = line;
			return (1);
		}
		*cursor++ = c;
	}
}
