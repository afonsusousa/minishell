/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:37:17 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:37:18 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

bool	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v'
		|| c == '\f');
}

bool	is_slash(char c)
{
	return (c == '/');
}

void	free_until_null(char ***str_v)
{
	size_t	i;

	i = 0;
	if (!*str_v)
		return ;
	while ((*str_v)[i])
		free((*str_v)[i++]);
	free(*str_v);
	*str_v = NULL;
}

char	*strjoin_three(char *s1, char *s2, char *s3)
{
	int		i;
	char	*ret;

	i = 0;
	ret = malloc(ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1);
	if (!ret)
		return (NULL);
	if (!s1 || !s2 || !s3)
		return (NULL);
	while (*s1)
		ret[i++] = *s1++;
	while (*s2)
		ret[i++] = *s2++;
	while (*s3)
		ret[i++] = *s3++;
	ret[i] = 0;
	return (ret);
}

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

bool	is_valid(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
			&& c <= '9') || c == '_');
}

char	**str_arr_append(char **arr, const char *s)
{
	size_t	len;
	char	**res;

	len = 0;
	if (!s)
		return (arr);
	while (arr && arr[len])
		len++;
	res = ft_calloc(len + 2, sizeof(char *));
	if (!res)
		return (NULL);
	for (size_t i = 0; i < len; i++)
		res[i] = arr[i];
	res[len] = ft_strdup(s);
	if (!res[len])
		return (free(res), NULL);
	res[len + 1] = NULL;
	if (arr)
		free(arr);
	return (res);
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
