//
// Created by afonsusousa on 9/24/25.
//

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../includes/lexer.h"
#include "../includes/tokens.h"
#include "../includes/minishell.h"

void token_stream_init(t_minishell *sh, size_t capacity)
{
    if (capacity == 0)
        capacity = 32;
    sh->ts->data = (t_token *)calloc(capacity, sizeof(t_token));
    if (sh->ts->data)
        sh->ts->capacity = capacity;
    else
        sh->ts->capacity = 0;
    sh->ts->count = 0;
    sh->ts->position = 0;
    sh->ts->read_position = 0;
    sh->ts->tk = NULL;
}

void token_stream_free(t_token_stream *ts)
{
    size_t  i;

    i = 0;
    while (i < ts->count)
    {
        if (ts->data[i].type == TOK_WORD
            || ts->data[i].type == TOK_ASSIGNMENT_WORD
            || ts->data[i].type == TOK_APPEND_WORD)
            free(ts->data[i].lexeme);
        i++;
    }
    if (ts->data)
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

