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
 * t_word lore:
 * [In the beginning, there was the Word...]
 *
 * Because lexer defined the Word as simply space-separated sequences
 * of characters (although respecting quotes), this story came to be.
 *
 * In the first commits, expansions lived at word level, blissfully
 * ignorant of what argv would expect of them... That it would chop them up,
 * that it would split them from their fellow compatriots, free and realloc them,
 * outcast them from one another... And that this would all come to pass
 * by the cruel hand of their own, expansive, nature...
 * In that frivolous dawn, this program would do something like:
 *
 * > export X="     A     B    " -> yes, those are multiple spaces
 * > echo "1"$X"2"
 * "1     A     B    2"
 * ^ echo received ["1     A     B    2"] as argv
 *
 * But, as the testers had spoken in the blood-red of their lines,
 * their shade forever cast onto these .c files, the quotes knew...
 * No two of them would come to trust one another, for a long time.
 *
 * So they consulted with Bash, the twice born, whom, in its cryptic
 * logic, sang them tales of an unmistakable but swiftly fading sense,
 * as if they had just woken up from a dream...
 *
 * > export X="   A    B   "
 * > echo "1"$X"2" -> lexer considers "1"$X"2" as one word...
 * "1 A B 2"
 * ^ but echo received ["1", "A", "B", "2"] as argv!
 *
 * What was that? Something about a... refactor? Confused and waking up
 * to a horizon threatened by command setup's light, they once again stared
 * into each other's eyes and, in that reflection, a new day dawned on them:
 * While Bash may have been but a fantasy, ft_split had become their reality.
 * And so they experimented, this time with even more quotes:
 *
 * > export X="'     A     B    '"
 * > echo "1"$X"2"
 * "1' A B '2"
 * ^ echo received ["1'", "A", "B", "'2"]
 * This was because the word was split on spaces
 * from its expansion "1'     A     B    '2"
 *
 * Somehow it all seemed to be working!
 *
 * Words, Spaces, Quotes, Variables... For most of the cases,
 * all these expansions were living together in Harmony...
 * But then everything changed, when the quote expansion attacked:
 *
 * > export X="     A      B     "
 * > echo "1      2"$X"3     4"
 * "1      2 A B 3     4"
 * ^ echo received ["1      2", "A", "B", "3     4"],
 * but minishell would send it ["1","2","A","B","3","4"]...
 *
 * ft_split wasn't going to cut it, it would split them further apart!
 * When the Word needed it the most, the expansion correctness
 * disappeared...
 *
 * [...]
 *
 * A hundred commits had passed, and my brother and I
 * discovered the new workaround, a user-defined type, named t_word.
 * Although the data structure was promising in its quote awareness,
 * it had a lot of helpers to be written, before it could save the lexer
 * from its simplicity. But I, for one, believe. t_word... can save the Word...
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
