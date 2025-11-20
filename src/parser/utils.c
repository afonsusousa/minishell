//
// Created by afonsusousa on 11/4/25.
//

#include <stddef.h>

#include "../../includes/parser.h"

t_token_type	is_redir_token_type(t_token_stream *ts)
{
    if (ts_match(ts, TOK_REDIR_IN))
        return (TOK_REDIR_IN);
    if (ts_match(ts, TOK_REDIR_OUT))
        return (TOK_REDIR_OUT);
    if (ts_match(ts, TOK_REDIR_APPEND))
        return (TOK_REDIR_APPEND);
    if (ts_match(ts, TOK_HEREDOC))
        return (TOK_HEREDOC);
    return (0);
}

int	is_redir_ahead(const t_token_stream *ts)
{
    return (ts_check(ts, TOK_REDIR_IN)
        || ts_check(ts, TOK_REDIR_OUT)
        || ts_check(ts, TOK_REDIR_APPEND)
        || ts_check(ts, TOK_HEREDOC));
}