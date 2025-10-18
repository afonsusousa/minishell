#ifndef MINISHELL_TOKENS_H
#define MINISHELL_TOKENS_H

#include <stddef.h>
#include <stdbool.h>
#include "lexer.h" // for t_token and token types

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

static inline void ts_read_token(t_token_stream *ts)
{
    if (ts->read_position >= ts->count && ts->count > 0) {
        ts->tk = &ts->data[ts->count - 1];
    } else if (ts->count > 0) {
        ts->tk = &ts->data[ts->read_position];
    } else {
        ts->tk = NULL;
    }
    ts->position = ts->read_position;
    ts->read_position += 1;
}

static inline const t_token *ts_peek(const t_token_stream *ts)
{
    if (ts->read_position >= ts->count && ts->count > 0)
        return (&ts->data[ts->count - 1]);
    return &ts->data[ts->read_position];
}

static inline const t_token *ts_prev(const t_token_stream *ts)
{
    size_t idx;

    if (ts->position == 0 || ts->count == 0)
        return NULL;
    idx = ts->position - 1;
    if (idx >= ts->count)
        idx = ts->count - 1;
    return &ts->data[idx];
}

static inline int ts_at_end(const t_token_stream *ts)
{
    if (ts->tk)
        return (ts->tk->type == TOK_EOF);
    return (ts_peek(ts)->type == TOK_EOF);
}

static inline const t_token *ts_advance(t_token_stream *ts)
{
    const t_token *prev = ts->tk;
    ts_read_token(ts);
    return prev;
}

static inline int ts_check(const t_token_stream *ts, t_token_type type)
{
    return ts_peek(ts)->type == type;
}

static inline int ts_match(t_token_stream *ts, t_token_type type)
{
    if (ts_check(ts, type)) { ts_read_token(ts); return 1; }
    return 0;
}

int ts_expect(t_token_stream *ts, t_token_type type, const char *what, char *errbuf, size_t errcap);

// Builder API (implemented in src/token_stream.c)
void token_stream_init(t_minishell *sh, size_t capacity);
void token_stream_free(t_token_stream *ts);
bool token_stream_reserve(t_token_stream *ts, size_t needed);
bool token_stream_push(t_token_stream *ts, const t_token *token);
bool token_stream_fill(t_token_stream *ts, t_lexer *lexer);

#endif // MINISHELL_TOKENS_H
