/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   abort.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:04:40 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:17:38 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/parser.h"

void	parser_abort(t_minishell *sh)
{
	sh->aborted_parse = true;
}

void	parser_abort_error(t_minishell *sh)
{
    const char  *name;

	name = get_token_type_name(ts_peek(sh->ts));
	print_syntax_error(name);
	sh->last_status = 2;
	parser_abort(sh);
}