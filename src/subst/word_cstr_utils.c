/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_cstr_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:27:33 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 18:57:55 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/globbing.h"
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"
#include <stdlib.h>

char	*word_to_cstr(const t_word *word)
{
	if (!word)
		return (NULL);
	return (ft_strdup(word->content));
}

char	**word_to_cstr_array(t_word **words)
{
	char	**ret;
	char	**dst;

	if (!words)
		return (NULL);
	ret = ft_calloc(sizeof(char *), (word_array_len(words) + 1));
	if (!ret)
		return (NULL);
	dst = ret;
	while (*words)
	{
		*dst++ = word_to_cstr(*words++);
		if (!*(dst - 1))
			return (free_until_null(&ret), NULL);
	}
	*dst = NULL;
	return (ret);
}

t_word	**word_array_append_cstr(t_word **arr, const char *content,
		bool all_quoted)
{
	t_word	**ret;
	t_word	**dst;
	t_word	**og;
	t_word	*word;

	if (!content)
		return (arr);
	word = word_new(content, all_quoted);
	if (!word)
		return (NULL);
	ret = ft_calloc(sizeof(t_word *), (word_array_len(arr) + 2));
	if (!ret)
		return (word_free(word), NULL);
	dst = ret;
	og = arr;
	while (arr && *arr)
		if (!word_dup_assign(dst++, *arr++))
			return (word_free_until_null(ret), word_free_until_null(og),
				word_free(word), NULL);
	*dst++ = word;
	*dst = NULL;
	return (word_free_until_null(og), ret);
}
