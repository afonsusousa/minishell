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

t_ast_list	*ast_list_push(t_ast_list **head, t_ast *ast_node)
{
	t_ast_list	*new_list_node;
	t_ast_list	*current;

	if (head == NULL || ast_node == NULL)
		return NULL;
	new_list_node = ast_list_new(ast_node);
	if (new_list_node == NULL)
		return NULL;
	if (*head == NULL)
	{
		*head = new_list_node;
		return new_list_node;
	}
	current = *head;
	while (current->next != NULL)
		current = current->next;
	current->next = new_list_node;
	return new_list_node;
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
