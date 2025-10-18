/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell blueprint <student@42>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 00:00:00 by executor          #+#    #+#             */
/*   Updated: 2025/10/07 00:00:00 by executor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_EXECUTOR_H
# define MINISHELL_EXECUTOR_H

# include <stddef.h>
# include <stdbool.h>
# include "ast.h"
# include "minishell.h"
# include "envp.h"

int	exec_ast(t_minishell *sh);

int	exec_command_line(t_minishell *sh, t_ast *node);
int	exec_binop(t_minishell *sh, t_ast *node); /* OR / AND */
int	exec_pipeline(t_minishell *ctx, const t_ast_list *cmds);
int	exec_command(t_minishell *ctx, t_ast *node, bool in_fork);
int	exec_simple_command(t_minishell *ctx, t_ast *node, bool in_fork);
int	exec_grouping(t_minishell *ctx, t_ast *node, bool in_fork);

char	**words_to_argv(t_minishell *sh, t_ast_list *words);
void	free_argv(char **argv);

#endif /* MINISHELL_EXECUTOR_H */
