/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftpp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:37:17 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 18:58:38 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"
#include <stddef.h>
#include <stdlib.h>

char	*ft_strnjoin(const char *s1, const char *s2, size_t scnd)
{
	char	*ret;
	char	*og;

	ret = ft_calloc(ft_strlen(s1) + scnd + 1, sizeof(char));
	og = ret;
	if (!ret)
		return (NULL);
	while (s1 && *s1)
		*ret++ = *s1++;
	while (s2 && *s2 && scnd--)
		*ret++ = *s2++;
	*ret = 0;
	return (og);
}

char	*ft_strndup(const char *str, size_t size)
{
	char	*t;
	char	*r;

	if (!str)
		return (NULL);
	t = (char *)malloc(size + 1);
	if (!t)
		return (NULL);
	r = t;
	while ((size_t)(t - r) < size && *str)
		*t++ = *str++;
	*t = 0;
	return (r);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 && !s2)
		return (0);
	if (!s1)
		return (-1);
	if (!s2)
		return (1);
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int	ft_memucmp(void *s1, void *s2)
{
	return (ft_strcmp((const char *)s1, (const char *)s2));
}

char	**ft_arrjoin(char **s1, char **s2)
{
	char	**ret;
	size_t	len1;
	size_t	len2;
	size_t	i;

	len1 = cstr_arr_len(s1);
	len2 = cstr_arr_len(s2);
	ret = ft_calloc(len1 + len2 + 1, sizeof(char *));
	if (!ret)
		return (NULL);
	i = -1;
	while (++i < len1)
	{
		ret[i] = ft_strdup(s1[i]);
		if (!ret[i])
			return (free_until_null((char ***)&ret), NULL);
	}
	i = -1;
	while (++i < len2)
	{
		ret[len1 + i] = ft_strdup(s2[i]);
		if (!ret[len1 + i])
			return (free_until_null((char ***)&ret), NULL);
	}
	return (ret);
}
