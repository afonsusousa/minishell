#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"
#include "utils.h"

#include "../../includes/parser.h"

#include "minishell.h"

t_ast	*parse_command_line(t_parser *p)
{
    int		term;
    t_ast	*list_node;

    list_node = parse_list(p);
    if (!list_node)
        return (NULL);
    term = 0;
    if (ts_match(&p->ts, TOK_SEMI))
        term = ';';
    else if (ts_match(&p->ts, TOK_AMP))
        term = '&';
    return (ast_make_command_line_node(list_node, term));
}

void parse(t_minishell *sh)
{
	t_parser	p = {0};

	p.ts.tokens = sh->ts->tokens;
	p.ts.count = sh->ts->count;
	p.ts.capacity = sh->ts->count;
	p.ts.position = 0;
	p.ts.read_position = 0;
	p.ts.tk = NULL;
	sh->ast = parse_command_line(&p);
}
