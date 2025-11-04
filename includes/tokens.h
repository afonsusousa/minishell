#ifndef MINISHELL_TOKENS_H
#define MINISHELL_TOKENS_H

#include <stddef.h>
#include "lexer.h"

typedef struct s_minishell t_minishell;
typedef struct s_token_stream
{
    t_token  *data;
    size_t    count;
    size_t    capacity;
    size_t    position;
    size_t    read_position;
    const t_token *tk;
}   t_token_stream;

// Stream construction
void            token_stream_init(const t_minishell *sh, size_t capacity);
void            token_stream_free(t_token_stream *ts);
bool            token_stream_reserve(t_token_stream *ts, size_t needed);
bool            token_stream_push(t_token_stream *ts, const t_token *token);
bool            token_stream_fill(t_token_stream *ts, t_lexer *lexer);

// Stream API
const t_token   *ts_peek(const t_token_stream *ts);
const t_token   *ts_prev(const t_token_stream *ts);
const t_token   *ts_advance(t_token_stream *ts);
int             ts_check(const t_token_stream *ts, t_token_type type);
int             ts_match(t_token_stream *ts, t_token_type type);

#endif // MINISHELL_TOKENS_H
