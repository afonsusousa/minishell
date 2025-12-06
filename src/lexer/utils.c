/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 03:29:15 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/06 00:00:00 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include "libft.h"

#include "../../includes/lexer.h"
#include "../../includes/utils.h"

int	is_meta(char c)
{
	return (c == '|' || c == '&' || c == ';'
		|| c == '(' || c == ')' || c == '<' || c == '>');
}

int	is_break(char c)
{
	return (c == '\0' || is_space(c) || is_meta(c));
}

t_token	*token_new(const t_token_type type)
{
	t_token	*token;

	token = calloc(1, sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->type = type;
	return (token);
}
