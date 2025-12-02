#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"
#include "utils.h"

#include "../../includes/parser.h"

#include "minishell.h"

t_ast	*parse_command_line(t_minishell *sh, bool subshell)
{
    int		term;
    t_ast	*node;

    if (sh->aborted_parse || ts_match(sh->ts, TOK_EOF))
        return (NULL);
    node = parse_logical(sh);
    if (!node || sh->aborted_parse)
        return (NULL);
    term = 0;
    if (ts_match(sh->ts, TOK_SEMI))
        term = ';';
    else if (ts_match(sh->ts, TOK_AMP))
        term = '&';
    else if (!subshell && !ts_match(sh->ts, TOK_EOF))
        return(parser_abort_error(sh), ast_free(node), NULL);
    return (ast_make_command_line_node(node, term));
}

void parse(t_minishell *sh)
{
    if (sh->aborted_parse)
    {
        ast_free(sh->ast);
        sh->ast = NULL;
        return ;
    }
    sh->ast = parse_command_line(sh, false);
}
