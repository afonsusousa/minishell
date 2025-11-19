//
// Created by afonsusousa on 11/4/25.
//

#include "minishell.h"
#include "../../includes/parser.h"

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
        if (!ast_list_push(&pipeline->as.pipeline.cores, core))
            return (ast_free(core), ast_free(pipeline), NULL);
    }
    return (pipeline);
}