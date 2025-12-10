//
// Created for tracked string utilities
//

#include "../../includes/sm.h"
#include "../../includes/utils.h"
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
	while (i <= ts->len)
	{
		new_ts->quoted_map[i] = ts->quoted_map[i];
		i++;
	}
	return (new_ts);
}

void	word_free_until_null(t_word **splits)
{
	size_t	i;

	if (!splits)
		return ;
	i = 0;
	while (splits[i])
	{
		word_free(splits[i]);
		i++;
	}
	free(splits);
}

t_word	**word_array_join(t_word **a, t_word **b)
{
	t_word	**result;
	size_t	a_len;
	size_t	b_len;
	size_t	i;

	a_len = 0;
	while (a && a[a_len])
		a_len++;
	b_len = 0;
	while (b && b[b_len])
		b_len++;
	result = malloc(sizeof(t_word *) * (a_len + b_len + 1));
	if (!result)
		return (NULL);
	i = -1;
	while (a && ++i < a_len)
		result[i] = a[i];
	i = -1;
	while (b && ++i < b_len)
		result[a_len + i] = b[i];
	result[a_len + b_len] = NULL;
	if (a)
		free(a);
	if (b)
		free(b);
	return (result);
}

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
	result = malloc(sizeof(t_word *) * (len + 2));
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = arr[i];
		i++;
	}
	result[len] = word_dup(word);
	result[len + 1] = NULL;
	if (arr)
		free(arr);
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
	len = 0;
	while (arr && arr[len])
		len++;
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
