/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrays.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:54:38 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 18:54:56 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/libft/libft.h"
#include <stddef.h>
#include <stdlib.h>

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

char	**cstr_arr_append(char **arr, const char *s)
{
	size_t	len;
	char	**res;
	char	**src;
	char	**dst;

	len = 0;
	if (!s)
		return (arr);
	while (arr && arr[len])
		len++;
	res = ft_calloc(len + 2, sizeof(char *));
	if (!res)
		return (NULL);
	src = arr;
	dst = res;
	while (src && *src)
		*dst++ = *src++;
	*dst = ft_strdup(s);
	if (!*dst)
		return (free(res), NULL);
	if (arr)
		free(arr);
	return (res);
}

size_t	cstr_arr_len(char **arr)
{
	size_t	len;

	len = 0;
	while (arr && arr[len])
		len++;
	return (len);
}

char	**cstr_arr_join(char **s1, char **s2)
{
	char	**res;
	char	**ptr;

	res = ft_calloc(cstr_arr_len(s1)
			+ cstr_arr_len(s2) + 1,
			sizeof(char *));
	if (!res)
		return (NULL);
	ptr = res;
	while (s1 && *s1)
		*ptr++ = *s1++;
	while (s2 && *s2)
		*ptr++ = *s2++;
	return (res);
}
