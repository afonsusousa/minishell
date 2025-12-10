/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:36:17 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:24:57 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser.h"
#include "minishell.h"

t_ast	*parse_pipeline(t_minishell *sh)
{
	t_ast	*core;
	t_ast	*pipeline;

	if (sh->aborted_parse)
		return (NULL);
	core = parse_core(sh);
	if (!core || sh->aborted_parse)
		return (NULL);
	pipeline = ast_make_pipeline_node(core);
	if (!pipeline)
		return (NULL);
	while (!sh->aborted_parse && ts_match(sh->ts, TOK_PIPE))
	{
		core = parse_core(sh);
		if (!core || sh->aborted_parse)
			return (ast_free(pipeline), NULL);
		if (!ast_list_push(&pipeline->u_as.s_pipeline.cores, core))
			return (ast_free(core), ast_free(pipeline), NULL);
	}
	return (pipeline);
}
