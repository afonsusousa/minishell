/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:36:46 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:45:34 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/globbing.h"
#include "../../includes/minishell.h"
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"
#include <math.h>
#include <stdlib.h>

char	*expand_tilde(const t_minishell *sh, char *cmd)
{
	char	*home;
	char	*expanded;

	if (cmd[0] != '~')
		return (NULL);
	home = envp_getvar_value(sh->env, "HOME", sh->last_status);
	if (!home)
		return (ft_strdup(cmd));
	if (cmd[1] == '\0')
		expanded = ft_strdup(home);
	else if (cmd[1] == '/')
		expanded = strjoin_three(home, "", &cmd[1]);
	else
		expanded = ft_strdup(cmd);
	free(home);
	if (!expanded)
		return (ft_strdup(cmd));
	return (expanded);
}

static bool	has_unquoted_wildcard(const t_word *ts)
{
	char	*star;

	if (!ts || !ts->content)
		return (false);
	star = ft_strchr(ts->content, '*');
	while (star && ts->quoted_map[star - ts->content])
		star = ft_strchr(star + 1, '*');
	return (star != NULL);
}

static t_word	**build_result(t_word *exp_result, t_word **matches)
{
	t_word	**result;
	int		size;
	char	**cstr_array;
	char	**ptr;

	if (matches && *matches)
	{
		size = 0;
		while (matches[size])
			size++;
		cstr_array = word_to_cstr_array(matches);
		if (!cstr_array)
			return (NULL);
		merge_sort_strings(cstr_array, 0, size - 1);
		result = NULL;
		ptr = cstr_array;
		while (*ptr)
		{
			result = word_array_append_cstr(result, *ptr++, true);
			if (!result)
				free_until_null(&cstr_array);
		}
		free(cstr_array);
		word_free_until_null(matches);
	}
	else
		result = word_array_append_word(NULL, exp_result);
	return (result);
}

t_word	**expand_cwd_wildcards(t_word *word)
{
	t_word	**splits;
	t_word	**matches;

	if (!word || !word->content)
		return (NULL);
	if (!has_unquoted_wildcard(word))
		return (word_array_append_word(NULL, word));
	splits = word_split(word, is_slash, false);
	if (!splits)
		return (NULL);
	if (splits[1])
		matches = get_matches(splits[0]->content, &splits[1]);
	else
		matches = get_matches("", splits);
	word_free_until_null(splits);
	if (!matches)
		return (word_array_append_word(NULL, word));
	return (matches);
}

t_word	**expand_argv_word(const t_minishell *sh, const char *word)
{
	char	*tilde_exp;
	t_word	*exp_word;
	t_word	**matches;
	t_word	**ret;

	if (!word)
		return (NULL);
	tilde_exp = expand_tilde((t_minishell *)sh, (char *)word);
	if (tilde_exp)
		exp_word = expanded(sh->env, tilde_exp, sh->last_status,
				EXPAND_VARS | CONSUME_QUOTES);
	else
		exp_word = expanded(sh->env, word, sh->last_status,
				EXPAND_VARS | CONSUME_QUOTES);
	free(tilde_exp);
	if (!exp_word)
		return (NULL);
	if (has_unquoted_wildcard(exp_word))
		matches = expand_cwd_wildcards(exp_word);
	else
		matches = NULL;
	ret = build_result(exp_word, matches);
	if (ret)
		return (word_free(exp_word), ret);
	return (word_free(exp_word), word_array_append_cstr(NULL, word, false));
}
