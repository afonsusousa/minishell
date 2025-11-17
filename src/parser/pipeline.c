//
// Created by afonsusousa on 11/4/25.
//

#include "minishell.h"
#include "../../includes/parser.h"

t_ast	*parse_pipeline(t_minishell *sh)
{
    t_ast	*core;
    t_ast	*pipeline;

    core = parse_core(sh);
    if (!core)
        return (NULL);
    pipeline = ast_make_pipeline_node(core);
    if (!pipeline)
        return (NULL);
    while (ts_match(sh->ts, TOK_PIPE))
    {
        core = parse_core(sh);
        if (!core)
            return (NULL);
        if (!ast_list_push(&pipeline->as.pipeline.cores, core))
            return (NULL);
    }
    return (pipeline);
}