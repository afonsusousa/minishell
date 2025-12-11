/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:32:36 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:39:14 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/executor.h"
#include "../../../../includes/globbing.h"
#include "../../../../includes/utils.h"
#include <stdlib.h>
#include <unistd.h>

/**
 * Because lexer defines words as simply space-seperated sequences
 * of characters (respecting quotes). The t_word type came about,
 * containing the expansion result and its quoting metadata.
 * This is the reason for all the shenanigans going on in the
 * expansion state machine.
 *
 * This need originally emerged in the following sequence:
 *
 * > export X="   A    B   "  -> yes, those are multiple spaces
 * > echo "1"$X"2" -> lexer considers "1"$X"2" as one word.
 * 1 A B 2 -> but echo received ["1", "A", "B", "2"] as &argv[1]
 *
 *  but
 *
 *  > export X="'     A     B    '"
 *  > echo "1"$X"2"
 *  1' A B '2
 **/
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
		expanded = expand_argv_word(sh, *words++);
		expanded = split_expanded(expanded);
		if (!expanded)
			continue ;
		i = 0;
		while (expanded[i])
			i++;
		*argc += i;
		argv_words = word_array_join(argv_words, expanded);
	}
	argv = word_to_cstr_array(argv_words);
	word_free_until_null(argv_words);
	return (argv);
}

void	free_argv(char **argv)
{
	size_t	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
		free(argv[i++]);
	free(argv);
}
