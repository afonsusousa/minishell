/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:39:53 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 18:59:56 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "ast.h"
# include "tokens.h"
# include <stddef.h>

// Recursive Descent Parser
void			parse(t_minishell *sh);
t_ast			*parse_command_line(t_minishell *sh, bool subshell);
t_ast			*parse_logical(t_minishell *sh);
t_ast			*parse_pipeline(t_minishell *sh);
t_ast			*parse_core(t_minishell *sh);
t_ast			*parse_command(t_minishell *sh);
const char		**parse_assignments(t_minishell *sh);
t_ast			*parse_redir(t_minishell *sh);
t_ast			*parse_grouping(t_minishell *sh);
t_ast_list		*parse_core_redirs(t_minishell *sh);

// Utils
t_token_type	is_redir_token_type(t_token_stream *ts);
int				is_redir_ahead(const t_token_stream *ts);
int				heredoc_setup(t_minishell *sh, int heredoc[2]);
int				heredoc_descend(t_minishell *sh, t_ast *node);

// Helpers
t_ast			*ast_make_binary_node(t_ast_type type, t_ast *left,
					t_ast *right);
t_ast			*ast_make_command_line_node(t_ast *list, int terminator);
t_ast			*ast_make_pipeline_node(t_ast *first_core);
t_ast			*ast_make_grouping_node(t_ast *list);
t_ast			*ast_make_redir_node(t_token_type type);
t_ast			*ast_make_command_node(const char **assignments, char **argv,
					int argc, t_ast_list *redirs);

// Error
const char		*get_token_type_name(const t_token *tk);
void			print_syntax_error(const char *token_name);
void			parser_abort(t_minishell *sh);
void			parser_abort_error(t_minishell *sh);

#endif // MINISHELL_PARSER_H
