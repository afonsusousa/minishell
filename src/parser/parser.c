#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"
#include "utils.h"

#include "../../includes/parser.h"

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

t_ast	*parse(const t_token *tokens, const size_t count)
{
	t_parser	p = {0};

	p.ts.data = (t_token *)tokens;
	p.ts.count = count;
	p.ts.capacity = count;
	p.ts.position = 0;
	p.ts.read_position = 0;
	p.ts.tk = NULL;
	return (parse_command_line(&p));
}
