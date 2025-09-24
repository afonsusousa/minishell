#ifndef MINISHELL_TOKENS_H
#define MINISHELL_TOKENS_H

#include <stddef.h>

typedef struct s_token_stream 
{
    const t_token *tokens;
    size_t         count;
    size_t         pos;
}   t_token_stream;

static inline const t_token *ts_peek(const t_token_stream *ts)
{
    if (ts->pos >= ts->count) 
        return (&ts->tokens[ts->count - 1]);
    return &ts->tokens[ts->pos];
}

static inline const t_token *ts_prev(const t_token_stream *ts) 
{
    if (ts->pos == 0) 
        return (NULL);
    return (&ts->tokens[ts->pos - 1]);
}

static inline int ts_at_end(const t_token_stream *ts) 
{
    return (ts_peek(ts)->type == TOK_EOF);
}

static inline const t_token *ts_advance(t_token_stream *ts) 
{
    if (!ts_at_end(ts)) 
        ts->pos++;
    return ts_prev(ts);
}

static inline int ts_check(const t_token_stream *ts, t_token_type type) 
{
    return ts_peek(ts)->type == type;
}

static inline int ts_match(t_token_stream *ts, t_token_type type) 
{
    if (ts_check(ts, type)) { ts_advance(ts); return 1; }
    return 0;
}

int ts_expect(t_token_stream *ts, t_token_type type, const char *what, char *errbuf, size_t errcap);

#endif // MINISHELL_TOKENS_H

