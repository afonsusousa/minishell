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
	char	**result;
	size_t	count;
	size_t	i;

	if (!words)
		return (NULL);
	count = 0;
	while (words[count])
		count++;
	result = ft_calloc(sizeof(char *), (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < count)
	{
		result[i] = word_to_cstr(words[i]);
		if (!result[i])
			free_until_null(&result);
		i++;
	}
	result[count] = NULL;
	return (result);
}

t_word	**word_array_append_cstr(t_word **arr, const char *content,
		bool all_quoted)
{
	t_word	**result;
	t_word	*word;
	size_t	len;
	size_t	i;

	if (!content)
		return (arr);
	word = word_new(content, all_quoted);
	if (!word)
		return (NULL);
	len = word_array_len(arr);
	result = malloc(sizeof(t_word *) * (len + 2));
	if (!result)
		return (word_free(word), NULL);
	i = 0;
	while (i < len)
	{
		result[i] = arr[i];
		i++;
	}
	result[len] = word;
	result[len + 1] = NULL;
	if (arr)
		free(arr);
	return (result);
}
