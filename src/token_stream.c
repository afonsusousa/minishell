//
// Created by afonsusousa on 9/24/25.
//

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "../includes/lexer.h"
#include "../includes/tokens.h"

void token_stream_init(t_token_stream *ts, size_t capacity)
{
    if (capacity == 0)
        capacity = 32;
    ts->data = (t_token *)calloc(capacity, sizeof(t_token));
    if (ts->data)
        ts->capacity = capacity;
    else
        ts->capacity = 0;
    ts->count = 0;
    ts->position = 0;
    ts->read_position = 0;
    ts->tk = NULL;
}

void token_stream_free(t_token_stream *ts)
{
    free(ts->data);
    memset(ts, 0, sizeof(t_token_stream));
}

bool    token_stream_reserve(t_token_stream *ts, size_t needed)
{
    size_t to_reserve;
    t_token *new_data;

    if (ts->capacity >= needed)
        return (true);
    to_reserve = ts->capacity ? ts->capacity : 32;
    while (to_reserve < needed)
        to_reserve <<= 1;
    new_data = (t_token *)calloc(to_reserve, sizeof(t_token));
    if (!new_data)
        return (false);
    if (ts->data && ts->count)
        memcpy(new_data, ts->data, ts->count * sizeof(t_token));
    free(ts->data);
    ts->data = new_data;
    ts->capacity = to_reserve;
    return (true);
}

bool token_stream_push(t_token_stream *ts, const t_token *token)
{
    if (!token_stream_reserve(ts, ts->count + 1))
        return (false);
    ts->data[ts->count++] = *token;
    return (true);
}

bool token_stream_fill(t_token_stream *ts, t_lexer *lexer)
{
    t_token *heap_tok = NULL;

    heap_tok = lexer_next_token(lexer);
    while (true)
    {
        if (!token_stream_push(ts, heap_tok))
            return (free(heap_tok), false);
        if (heap_tok->type == TOK_EOF)
        {
            free(heap_tok);
            break;
        }
        free(heap_tok);
        heap_tok = lexer_next_token(lexer);
    }
    ts->position = 0;
    ts->read_position = 0;
    ts->tk = NULL;
    return (true);
}

int ts_expect(t_token_stream *ts, t_token_type type, const char *what, char *errbuf, size_t errcap)
{
    if (ts_match(ts, type))
        return 1;
    if (errbuf && errcap)
        snprintf(errbuf, errcap, "expected %s, got token type %d", what ? what : "token", (int)ts_peek(ts)->type);
    return 0;
}
