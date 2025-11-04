//
// Created by afonsusousa on 11/4/25.
//

#include "../../includes/parser.h"

t_ast	*parse_pipeline(t_parser *p)
{
    t_ast	*core;
    t_ast	*pipeline;

    core = parse_core(p);
    if (!core)
        return (NULL);
    pipeline = ast_make_pipeline_node(core);
    if (!pipeline)
        return (NULL);
    while (ts_match(&p->ts, TOK_PIPE))
    {
        core = parse_core(p);
        if (!core)
            return (NULL);
        if (!ast_list_push(&pipeline->as.pipeline.cores, core))
            return (NULL);
    }
    return (pipeline);
}