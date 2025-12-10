/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:33:38 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:21:52 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"
#include "../../includes/minishell.h"

int	exec_logical(t_minishell *sh, const t_ast *node)
{
	int left_status;

	if (!node || (node->type != AST_AND_LIST && node->type != AST_OR_LIST))
		return (1);
	left_status = exec_node(sh, node->u_as.s_binop.left);
	if (node->type == AST_AND_LIST)
	{
		if (left_status == 0)
			return (exec_node(sh, node->u_as.s_binop.right));
		return (left_status);
	}
	if (node->type == AST_OR_LIST)
	{
		if (left_status != 0)
			return (exec_node(sh, node->u_as.s_binop.right));
		return (left_status);
	}
	return (1);
}
