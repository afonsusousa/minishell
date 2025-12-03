//
// Created by afonsusousa on 12/1/25.
//

#include <stdlib.h>
#include <linux/limits.h>

#include "../../includes/sm.h"
#include "../../lib/libft/libft.h"

void sm_advance(t_quote_machine *sm)
{
    if (sm->str_pos < sm->str_len)
        sm->ch = sm->str[++sm->str_pos];
    else
        sm->ch = '\0';
}

void sm_trasition(t_quote_machine *sm, t_sm_state new)
{
    sm->prev = sm->curr;
    sm->curr = new;
    sm_advance(sm);
}

void sm_laststate(t_quote_machine *sm)
{
    sm->curr = sm->curr ^ sm->prev;
    sm->prev = sm->curr ^ sm->prev;
    sm->curr = sm->curr ^ sm->prev;
}

void sm_consume(t_quote_machine *sm)
{
    sm->buffer[sm->buff_pos] = sm->ch;
    sm->quoted_map[sm->buff_pos] = (sm->curr == IN_SQ || sm->curr == IN_DQ);
    sm->buff_pos++;
    sm_advance(sm);
}

void sm_cat(t_quote_machine *sm, const char *str)
{
    bool quoted;

    if (!str)
        return ;
    quoted = (sm->prev == IN_DQ || sm->prev == IN_SQ);
    while (*str && sm->buff_pos < ARG_MAX)
    {
        sm->buffer[sm->buff_pos] = *str++;
        sm->quoted_map[sm->buff_pos] = quoted;
        sm->buff_pos++;
    }
}

void sm_cat_word(t_quote_machine *sm, const t_word *word)
{
    size_t i;

    if (!word || !word->content)
        return ;
    i = 0;
    while (i < word->len && sm->buff_pos < ARG_MAX)
    {
        sm->buffer[sm->buff_pos] = word->content[i];
        if (word->quoted_map)
            sm->quoted_map[sm->buff_pos] = word->quoted_map[i];
        else
            sm->quoted_map[sm->buff_pos] = false;
        sm->buff_pos++;
        i++;
    }
}

void    sm_init(t_quote_machine *sm, const char *str)
{
    size_t i;

    sm->str = str;
    sm->ch = *str;
    sm->str_pos = 0;
    sm->buff_pos = 0;
    sm->curr = DEFAULT;
    sm->prev = DEFAULT;
    sm->str_len = ft_strlen(str);
    ft_bzero(sm->buffer, ARG_MAX);
    i = 0;
    while (i < ARG_MAX)
        sm->quoted_map[i++] = false;
}

