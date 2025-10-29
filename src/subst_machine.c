//
// Created by wlucas-f on 10/28/25.
//

#include <ctype.h>
#include <linux/limits.h>

#include "../includes/envp.h"
#include "../includes/utils.h"
#include "../lib/libft/libft.h"

typedef enum s_state
{
    DEFAULT = 0,
    IN_DQ,
    IN_SQ,
    IN_VAR
} t_state;

typedef struct s_quote_machine
{
    const char  *str;
    char         ch;
    size_t      str_pos;
    size_t      str_len;
    t_state     curr;
    t_state     prev;
} t_quote_machine;

void sm_advance(t_quote_machine *sm)
{
    if (sm->str_pos < sm->str_len)
        sm->ch = sm->str[++sm->str_pos];
    else
        sm->ch = '\0';
}

void sm_trasition(t_quote_machine *sm, t_state new)
{
    sm->prev = sm->curr;
    sm->curr = new;
    sm_advance(sm);
}

void sm_retrocede(t_quote_machine *sm)
{
    if (sm->str_pos)
        sm->ch = sm->str[--sm->str_pos];
    else
        sm->ch = sm->str[0];
}
void sm_laststate(t_quote_machine *sm)
{
    sm->curr = sm->curr ^ sm->prev;
    sm->prev = sm->curr ^ sm->prev;
    sm->curr = sm->curr ^ sm->prev;
}

const char *sm_getstr(t_quote_machine *sm)
{
    return (&(sm->str[sm->str_pos]));
}

char *expanded(const t_envp *env, const char *str)
{
    t_quote_machine sm;
    size_t  i;
    char buffer[ARG_MAX];
    const char *var;

    i = 0;
    sm.str = str;
    sm.curr = DEFAULT;
    sm.prev = DEFAULT;
    sm.str_len = ft_strlen(str);
    ft_bzero(buffer, ARG_MAX);
    while (sm.ch)
    {
        if (sm.curr == DEFAULT)
        {
            if (sm.ch == '\'')
               sm_trasition(&sm, IN_SQ);
            if (sm.ch == '"')
                sm_trasition(&sm, IN_DQ);
            if (sm.ch == '$')
                sm_trasition(&sm, IN_VAR);
            buffer[i++] = sm.ch;
        }
        else if (sm.curr == IN_DQ)
        {
            if (sm.ch == '$')
                sm_trasition(&sm, IN_VAR);
            if (sm.ch == '"')
                sm_trasition(&sm, DEFAULT);
            buffer[i++] = sm.ch;
        }
        else if (sm.curr == IN_SQ)
        {
            if (sm.ch == '\'')
                sm_trasition(&sm, DEFAULT);
            buffer[i++] = sm.ch;
        }
        else if (sm.curr == IN_VAR)
        {
            if (!is_valid(sm.ch))
            {
                while (!isspace(sm.ch))
                {
                    buffer[i++] = sm.ch;
                    sm_advance(&sm);
                }
                sm_laststate(&sm);
                continue ;
            }
            var = envp_getvar_value(env, sm_getstr(&sm));
            ft_strlcat(buffer, var, ft_strlen(buffer) + ft_strlen(var) + 1);
            sm_laststate(&sm);
        }
        sm_advance(&sm);
    }
    return (ft_strdup(buffer));
}