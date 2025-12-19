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
	t_word	**iter;

	iter = splits;
	if (!iter)
		return ;
	while (*iter)
		word_free(*iter++);
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
	t_word	**ret;
	t_word	**dst;
	t_word	**aa;
	t_word	**bb;

	aa = a;
	bb = b;
	ret = ft_calloc(sizeof(t_word *),
			(word_array_len(a) + word_array_len(b) + 1));
	if (!ret)
		return (NULL);
	dst = ret;
	while (a && *a)
		if (!word_dup_assign(dst++, *a++))
			return (word_free_until_null(ret), word_free_until_null(aa),
				word_free_until_null(bb), NULL);
	while (b && *b)
		if (!word_dup_assign(dst++, *b++))
			return (word_free_until_null(ret), word_free_until_null(aa),
				word_free_until_null(bb), NULL);
	*dst = NULL;
	return (word_free_until_null(aa),
		word_free_until_null(bb), ret);
}

t_word	**word_array_append_word(t_word **arr, const t_word *word)
{
	t_word	**og;
	t_word	**dst;
	t_word	**ret;

	if (!word)
		return (arr);
	dst = ft_calloc(sizeof(t_word *), (word_array_len(arr) + 2));
	ret = dst;
	og = arr;
	if (!dst)
		return (NULL);
	while (arr && *arr)
		if (!word_dup_assign(dst++, *arr++))
			return (word_free_until_null(ret), word_free_until_null(og), NULL);
	*dst++ = word_dup(word);
	*dst = NULL;
	return (word_free_until_null(og), ret);
}
