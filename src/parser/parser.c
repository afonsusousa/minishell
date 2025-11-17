#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"
#include "utils.h"

#include "../../includes/parser.h"

#include "minishell.h"

t_ast	*parse_command_line(t_minishell *sh)
{
    int		term;
    t_ast	*list_node;

    list_node = parse_or_list(sh);
    if (!list_node)
        return (NULL);
    term = 0;
    if (ts_match(sh->ts, TOK_SEMI))
        term = ';';
    else if (ts_match(sh->ts, TOK_AMP))
        term = '&';
    return (ast_make_command_line_node(list_node, term));
}

void parse(t_minishell *sh)
{
    sh->ast = parse_command_line(sh);
}
