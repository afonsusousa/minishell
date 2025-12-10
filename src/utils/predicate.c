/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   predicate.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:46:03 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 18:46:16 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

bool	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v'
		|| c == '\f');
}

bool	is_slash(char c)
{
	return (c == '/');
}

bool	is_valid(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
			&& c <= '9') || c == '_');
}

bool	has_char_fn(const char *s, bool (*predicate)(char))
{
	if (!s || !predicate)
		return (NULL);
	while (*s)
	{
		if (predicate(*s))
			return ((char *)s);
		s++;
	}
	return (NULL);
}
