/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:32:36 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:32:38 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/executor.h"
#include "../../../../includes/globbing.h"
#include "../../../../includes/minishell.h"
#include "../../../../includes/utils.h"
#include <stdlib.h>
#include <unistd.h>

static t_word	**split_expanded(t_word **expanded)
{
	t_word	**split;
	t_word	**temp;
	int		i;

	if (!expanded || !expanded[0])
		return (expanded);
	split = NULL;
	i = 0;
	while (expanded[i])
	{
		temp = word_split(expanded[i], is_space, true);
		if (temp)
			split = word_array_join(split, temp);
		word_free(expanded[i]);
		i++;
	}
	free(expanded);
	if (!split)
		return (NULL);
	return (split);
}

char	**argv_to_arr(const t_minishell *sh, const char **words, int *argc)
{
	t_word	**argv_words;
	t_word	**expanded;
	char	**argv;
	int		i;

	argv_words = NULL;
	*argc = 0;
	if (!words)
		return (NULL);
	while (words && *words)
	{
		expanded = expand_argv_word(sh, *words);
		expanded = split_expanded(expanded);
		if (!expanded)
		{
			words++;
			continue ;
		}
		i = 0;
		while (expanded[i])
			i++;
		*argc += i;
		argv_words = word_array_join(argv_words, expanded);
		words++;
	}
	argv = word_to_cstr_array(argv_words);
	word_free_until_null(argv_words);
	return (argv);
}

void	free_argv(char **argv)
{
	size_t i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
		free(argv[i++]);
	free(argv);
}