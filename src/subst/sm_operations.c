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
    sm->buffer[sm->buff_pos++] = sm->ch;
    sm_advance(sm);
}

void sm_cat(t_quote_machine *sm, const char *str)
{
    if (!str)
        return ;
    while (*str && sm->buff_pos < ARG_MAX)
        sm->buffer[sm->buff_pos++] = *str++;
}

void    sm_init(t_quote_machine *sm, const char *str)
{
    sm->str = str;
    sm->ch = *str;
    sm->str_pos = 0;
    sm->buff_pos = 0;
    sm->curr = DEFAULT;
    sm->prev = DEFAULT;
    sm->str_len = ft_strlen(str);
    ft_bzero(sm->buffer, ARG_MAX);
}

