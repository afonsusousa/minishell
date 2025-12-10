/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:38:09 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:38:16 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/executor.h"
#include "../includes/minishell.h"
#include "../includes/parser.h"
#include "../includes/sig.h"
#include "../lib/libft/libft.h"
#include <linux/limits.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	exec_line(t_minishell *sh)
{
	sh->aborted_parse = false;
	token_stream_init(sh);
	parse(sh);
	add_history(sh->line);
	free(sh->line);
	token_stream_free(sh->ts);
	// print_ast(sh->ast, 0);
	if (sh->aborted_parse || !sh->ast)
		return (sh->last_status);
	exec_ast(sh);
	ast_free(sh->ast);
	sh->ast = NULL;
	return (sh->last_status);
}

static char	*get_prompt(void)
{
	const char	*home;
	const char	*user;
	const char	*hostname;
	char		cwd[1024];
	char		buffer[1024];

	home = getenv("HOME");
	user = getenv("USER");
	hostname = getenv("HOSTNAME");
	if (!user || !hostname)
		return (ft_strdup("minishell> "));
	ft_bzero(buffer, 1024);
	getcwd(cwd, 1024);
	ft_strlcat(buffer, user, 1024);
	ft_strlcat(buffer, "@", 1024);
	ft_strlcat(buffer, hostname, 1024);
	ft_strlcat(buffer, ":", 1024);
	if (home && ft_strnstr(cwd, home, ft_strlen(home)) == cwd)
	{
		ft_strlcat(buffer, "~", 1024);
		ft_strlcat(buffer, cwd + ft_strlen(home), 1024);
	}
	else
		ft_strlcat(buffer, cwd, 1024);
	ft_strlcat(buffer, "> ", 1024);
	return (ft_strdup(buffer));
}

int	rl_loop(t_minishell *sh)
{
	char	*prompt;

	signal_setup();
	while (1)
	{
		prompt = get_prompt();
		sh->line = readline(prompt);
		if (sh->line == NULL)
			break ;
		if (*sh->line != '\0')
			exec_line(sh);
		sh->line = NULL;
		free(prompt);
	}
	minishell_free(sh);
	return (true);
}

int	notty_line(t_minishell *sh)
{
	ssize_t rd;
	char c;
	char *cursor;
	char *line;

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