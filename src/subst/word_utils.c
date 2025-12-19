/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:37:54 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 18:58:10 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/globbing.h"
#include "../../lib/libft/libft.h"
#include <stdlib.h>

void	word_free(t_word *ts)
{
	if (!ts)
		return ;
	free(ts->content);
	free(ts->quoted_map);
	free(ts);
}

t_word	*word_dup(const t_word *ts)
{
	t_word	*new_ts;
	size_t	i;

	if (!ts)
		return (NULL);
	new_ts = malloc(sizeof(t_word));
	if (!new_ts)
		return (NULL);
	new_ts->content = ft_strdup(ts->content);
	if (!new_ts->content)
		return (free(new_ts), NULL);
	new_ts->len = ts->len;
	new_ts->quoted_map = malloc(sizeof(bool) * (ts->len + 1));
	if (!new_ts->quoted_map)
		return (free(new_ts->content), free(new_ts), NULL);
	i = 0;
	while (i <= ts->len && ts->quoted_map)
	{
		new_ts->quoted_map[i] = ts->quoted_map[i];
		i++;
	}
	return (new_ts);
}

bool	word_dup_assign(t_word **dest, t_word *word)
{
	if (!dest)
		return (false);
	*dest = word_dup(word);
	if (!*dest)
		return (false);
	return (true);
}

t_word	*word_new(const char *content, bool all_quoted)
{
	t_word	*word;
	size_t	i;

	if (!content)
		return (NULL);
	word = malloc(sizeof(t_word));
	if (!word)
		return (NULL);
	word->content = ft_strdup(content);
	if (!word->content)
		return (free(word), NULL);
	word->len = ft_strlen(content);
	word->quoted_map = malloc(sizeof(bool) * (word->len + 1));
	if (!word->quoted_map)
		return (free(word->content), free(word), NULL);
	i = 0;
	while (i <= word->len)
	{
		word->quoted_map[i] = all_quoted;
		i++;
	}
	return (word);
}
