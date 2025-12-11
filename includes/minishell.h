/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:39:48 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/11 01:46:52 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "ast.h"
# include "envp.h"
# include "globbing.h"
# include "tokens.h"
# include <sys/types.h>

typedef struct s_word	t_word;

typedef struct s_token_stream
{
	t_token				*tokens;
	size_t				count;
	size_t				capacity;
	size_t				position;
	size_t				read_position;
	const t_token		*tk;
}						t_token_stream;

typedef struct s_pipeline
{
	pid_t				pids[1024];
	size_t				count;
	int					prev_read;
}						t_pipeline;

typedef struct s_heredoc
{
	int					fd[2];
	t_word				*del;
	pid_t				pid;
	int					status;
}						t_heredoc;

typedef struct s_minishell
{
	t_envp				*env;
	t_envp				*ctx;
	char				*prompt;
	char				*line;
	t_token_stream		*ts;
	t_ast				*ast;
	bool				aborted_parse;
	t_pipeline			pipeline;
	t_heredoc			heredoc;
	int					last_status;
}						t_minishell;

int						rl_loop(t_minishell *sh);
int						exec_line(t_minishell *sh);
int						notty_line(t_minishell *sh);
void					minishell_free(t_minishell *sh);

#endif // MINISHELL_H