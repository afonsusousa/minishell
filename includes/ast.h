/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:38:58 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:38:59 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_AST_H
# define MINISHELL_AST_H

# include "lexer.h"

struct s_ast;

typedef enum e_ast_type
{
	AST_COMMAND_LINE,
	AST_LIST,
	AST_OR_LIST,
	AST_AND_LIST,
	AST_PIPELINE,
	AST_COMMAND,
	AST_GROUPING,
	AST_REDIR,
}							t_ast_type;

typedef struct s_ast_list
{
	struct s_ast			*node;
	struct s_ast_list		*next;
}							t_ast_list;

typedef struct s_ast
{
	t_ast_type				type;
	union
	{
		struct
		{
			struct s_ast	*list;
			int				terminator;
		} command_line;

		struct
		{
			t_ast_list		*cores;
		} pipeline;

		struct
		{
			const char		**assignments;
			int				argc;
			const char		**argv;
			t_ast_list		*redirs;
		} command;

		struct
		{
			struct s_ast	*list;
			t_ast_list		*redirs;
		} grouping;

		struct
		{
			t_token_type	kind;
			union
			{
				const char	*file_name;
				int			heredoc[2];
			} target;
		} redir;

		struct
		{
			struct s_ast	*left;
			struct s_ast	*right;
		} binop;

	} as;
}							t_ast;

t_ast						*ast_new(t_ast_type type);
t_ast_list					*ast_list_push(t_ast_list **head, t_ast *node);
void						ast_list_push_list(t_ast_list **head,
								t_ast_list *list);
void						ast_list_free(t_ast_list *lst);
void						ast_free(t_ast *node);
void						print_ast(const t_ast *n, int d);

#endif // MINISHELL_AST_H
