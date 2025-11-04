/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by afonsusousa       #+#    #+#             */
/*   Updated: 2025/09/24 04:11:14 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ast.h"

t_ast	*ast_new(const t_ast_type type)
{
	t_ast	*n;

	n = calloc(1, sizeof(t_ast));
	if (n == NULL)
		return (NULL);
	n->type = type;
	return (n);
}

