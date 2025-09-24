/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by afonsusousa       #+#    #+#             */
/*   Updated: 2025/09/24 04:11:16 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ast.h"

static t_ast_list	*ast_list_new(t_ast *n)
{
	 t_ast_list	*ln;

	ln = malloc(sizeof(t_ast_list));
	if (ln == NULL)
		return (NULL);
	ln->node = n;
	ln->next = NULL;
	return (ln);
}

void	ast_list_push(t_ast_list **head, t_ast *node)
{
	t_ast_list	*ln;
	t_ast_list	*cur;

	if (node == NULL)
		return ;
	ln = ast_list_new(node);
	if (ln == NULL)
		return ;
	if (*head == NULL)
	{
		*head = ln;
		return ;
	}
	cur = *head;
	while (cur->next != NULL)
		cur = cur->next;
	cur->next = ln;
}

void	ast_list_free(t_ast_list *lst)
{
	t_ast_list	*n;

	while (lst != NULL)
	{
		n = lst->next;
		free(lst);
		lst = n;
	}
}
