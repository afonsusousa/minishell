/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:30:38 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:30:39 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdio.h>

static void	print_indent(int depth)
{
	for (int i = 0; i < depth; ++i)
		fputs("    ", stdout);
}

static const char	*redir_kind_name(t_token_type k)
{
	switch (k)
	{
	case TOK_REDIR_IN:
		return ("<");
	case TOK_REDIR_OUT:
		return (">");
	case TOK_REDIR_APPEND:
		return (">>");
	case TOK_REDIR_0_IN:
		return ("0<");
	case TOK_REDIR_1_IN:
		return ("1<");
	case TOK_REDIR_2_IN:
		return ("2<");
	case TOK_REDIR_0_OUT:
		return ("0>");
	case TOK_REDIR_1_OUT:
		return ("1>");
	case TOK_REDIR_2_OUT:
		return ("2>");
	case TOK_HEREDOC:
		return ("<<");
	default:
		return ("?");
	}
}

static void	print_strs(const char **strs)
{
	if (!strs)
	{
		printf("[N/A]\n");
		return ;
	}
	for (const char **s = strs; s && *s; s++)
	{
		printf("[\"%s\"]", *s);
		if (s[1])
			printf(" ");
	}
	printf("\n");
}

void	print_ast(const t_ast *n, int d)
{
	if (!n)
	{
		print_indent(d);
		printf("(null)\n");
		return ;
	}
	switch (n->type)
	{
	case AST_COMMAND_LINE:
		print_indent(d);
		printf("[command_line] terminator=%c\n",
			n->as.command_line.terminator ? n->as.command_line.terminator : '0');
		print_ast(n->as.command_line.list, d + 1);
		break ;
	case AST_OR_LIST:
		print_indent(d);
		printf("[or_list]\n");
		print_ast(n->as.binop.left, d + 1);
		print_ast(n->as.binop.right, d + 1);
		break ;
	case AST_AND_LIST:
		print_indent(d);
		printf("[and_list]\n");
		print_ast(n->as.binop.left, d + 1);
		print_ast(n->as.binop.right, d + 1);
		break ;
	case AST_PIPELINE:
	{
		print_indent(d);
		printf("[pipeline]\n");
		const t_ast_list *it = n->as.pipeline.cores;
		for (; it; it = it->next)
			print_ast(it->node, d + 1);
		break ;
	}
	case AST_COMMAND:
	{
		print_indent(d);
		printf("[command]\n");
		print_indent(d + 1);
		printf("[assigments]\t");
		print_strs(n->as.command.assignments);
		print_indent(d + 1);
		printf("[argv]\t\t");
		print_strs(n->as.command.argv);
		const t_ast_list *r = n->as.command.redirs;
		print_indent(d + 1);
		printf("[redirs]\t\t");
		if (!r)
			printf("[N/A]");
		for (; r; r = r->next)
		{
			print_ast(r->node, d + 2);
			if (r->next)
				printf(" ");
		}
		printf("\n");
		break ;
	}
	case AST_GROUPING:
		print_indent(d);
		printf("[grouping] \n");
		print_indent(d);
		printf("[\n");
		print_ast(n->as.grouping.list, d + 1);
		print_indent(d);
		printf("]\n");
		break ;
	case AST_REDIR:
		printf("[%s,", redir_kind_name(n->as.redir.kind));
		if (n->as.redir.target.file_name && n->as.redir.kind != TOK_HEREDOC)
			printf("\"%s\"]", n->as.redir.target.file_name);
		else
			printf("<missing>");
		break ;
	default:
		print_indent(d);
		printf("<unknown node %d>\n", (int)n->type);
		break ;
	}
}