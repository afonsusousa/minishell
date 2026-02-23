/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispose.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:38:00 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:37:54 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/envp.h"
#include "../includes/minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

void	minishell_free(t_minishell *sh)
{
	ast_free(sh->ast);
	if (sh->prompt)
	{
		free(sh->prompt);
	}
	if (sh->line)
	{
		free(sh->line);
	}
	free_envp(sh->env);
	token_stream_free(sh->ts);
}
