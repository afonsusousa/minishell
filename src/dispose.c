/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispose.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:38:00 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:38:01 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/envp.h"
#include "../includes/minishell.h"
#include <readline/readline.h>

void	minishell_free(t_minishell *sh)
{
	rl_clear_history();
	ast_free(sh->ast);
	free_envp(sh->env);
	token_stream_free(sh->ts);
}