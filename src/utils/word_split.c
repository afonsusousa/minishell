/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:37:22 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:29:26 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/sm.h"
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"
#include <stdlib.h>

static size_t	count_words_fn(const t_word *ts, bool (*is_sep)(char),
		bool quote_aware)
{
	size_t	count;
	size_t	i;
	bool	in_word;

	if (quote_aware && ts->len == 1 && ts->content[0] == '\0')
		return (1);
	count = 0;
	i = 0;
	in_word = false;
	while (ts->content[i])
	{
		if (is_sep(ts->content[i]) && (!quote_aware || !ts->quoted_map[i]))
		{
			if (in_word)
			{
				count++;
				in_word = false;
			}
		}
		else
			in_word = true;
		i++;
	}
	if (in_word)
		count++;
	return (count);
}

static size_t	skip_separators(const t_word *ts, size_t pos,
		bool (*is_sep)(char), bool quote_aware)
{
	while (ts->content[pos] && is_sep(ts->content[pos]))
	{
		if (quote_aware && ts->quoted_map[pos])
			break ;
		pos++;
	}
	return (pos);
}

static size_t	find_word_end(const t_word *ts, size_t pos,
		bool (*is_sep)(char), bool quote_aware)
{
	while (ts->content[pos])
	{
		if (is_sep(ts->content[pos]))
		{
			if (!quote_aware || !ts->quoted_map[pos])
				break ;
		}
		pos++;
	}
	return (pos);
}

static t_word	*create_split_part(t_word *ts, size_t start, size_t len)
{
	t_word	*ret;
	size_t	k;

	ret = malloc(sizeof(t_word));
	if (!ret)
		return (NULL);
	ret->content = ft_strndup(&ts->content[start], len);
	if (!ret->content)
		return (free(ret), NULL);
	ret->len = len;
	ret->quoted_map = ft_calloc(sizeof(bool), (len + 1));
	if (!ret->quoted_map)
		return (free(ret->content), free(ret), NULL);
	k = 0;
	while (k < len && ts->quoted_map)
	{
		ret->quoted_map[k] = ts->quoted_map[start + k];
		k++;
	}
	ret->quoted_map[len] = false;
	return (ret);
}

t_word	**word_split(t_word *ts, bool (*is_separator)(char), bool quote_aware)
{
	t_word	**ret;
	size_t	split_count;
	size_t	i;
	size_t	j;
	size_t	start;

	if (!ts || !ts->content || !is_separator)
		return (NULL);
	split_count = count_words_fn(ts, is_separator, quote_aware);
	ret = ft_calloc(sizeof(t_word *), (split_count + 1));
	if (!ret)
		return (NULL);
	i = 0;
	j = 0;
	while (i < split_count)
	{
		j = skip_separators(ts, j, is_separator, quote_aware);
		start = j;
		j = find_word_end(ts, j, is_separator, quote_aware);
		ret[i] = create_split_part(ts, start, j - start);
		if (!ret[i++])
			return (word_free_until_null(ret), NULL);
	}
	ret[split_count] = NULL;
	return (ret);
}
