/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_array_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:28:42 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 21:07:35 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/globbing.h"
#include <stdlib.h>

#include "libft.h"

void	word_free_until_null(t_word **splits)
{
	size_t	i;

	if (!splits)
		return ;
	i = 0;
	while (splits[i])
		word_free(splits[i++]);
	free(splits);
}

size_t	word_array_len(t_word **arr)
{
	size_t	len;

	len = 0;
	while (arr && arr[len])
		len++;
	return (len);
}

t_word	**word_array_join(t_word **a, t_word **b)
{
	t_word	**result;
	size_t	a_len;
	size_t	b_len;
	t_word	**dst;
	t_word	**a_bak;
	t_word	**b_bak;

	a_len = word_array_len(a);
	b_len = word_array_len(b);
	a_bak = a;
	b_bak = b;
	result = ft_calloc(sizeof(t_word *), (a_len + b_len + 1));
	if (!result)
		return (NULL);
	dst = result;
	while (a && *a)
		*dst++ = word_dup(*a++);
	while (b && *b)
		*dst++ = word_dup(*b++);
	*dst = NULL;
	word_free_until_null(a_bak);
	word_free_until_null(b_bak);
	return (result);
}

t_word	**word_array_append_word(t_word **arr, const t_word *word)
{
	t_word	**result;
	size_t	len;
	size_t	i;

	if (!word)
		return (arr);
	len = 0;
	while (arr && arr[len])
		len++;
	result = ft_calloc(sizeof(t_word *), (len + 2));
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = word_dup(arr[i]);
		i++;
	}
	word_free_until_null(arr);
	result[len] = word_dup(word);
	result[len + 1] = NULL;
	return (result);
}
