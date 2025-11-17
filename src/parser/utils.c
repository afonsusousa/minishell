//
// Created by afonsusousa on 11/4/25.
//

#include <stddef.h>

#include "../../includes/parser.h"

int	is_redir_token_type(t_token_type t)
{
    return (t == TOK_REDIR_IN
        || t == TOK_REDIR_OUT
        || t == TOK_REDIR_APPEND
        || t == TOK_HEREDOC);
}

int	is_redir_ahead(const t_token_stream *ts)
{
    const t_token *tk;

    tk = ts_peek(ts);
    return (tk != NULL && is_redir_token_type(tk->type));
}