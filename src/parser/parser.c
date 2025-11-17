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

    if (sh->aborted_parse)
        return (NULL);
    list_node = parse_or_list(sh);
    if (!list_node || sh->aborted_parse)
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
    if (sh->aborted_parse)
    {
        ast_free(sh->ast);
        sh->ast = NULL;
        return ;
    }
    sh->ast = parse_command_line(sh);
}
