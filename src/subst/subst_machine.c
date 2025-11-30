//
// Created by wlucas-f on 10/28/25.
//

#include <ctype.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "../../includes/envp.h"
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"
#include "../../includes/globbing.h"

typedef struct s_quote_machine
{
    t_state     curr;
    t_state     prev;
    const char  *str;
    size_t      str_pos;
    size_t      str_len;
    char        ch;
    char        buffer[ARG_MAX];
    size_t      buff_pos;
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

//TODO: is_valid needs fixing (allows much more characters!!)
char *expanded(const t_minishell *sh, const char *str, int flags)
{
    t_quote_machine sm;
    char *var;

    sm_init(&sm, str);
    while (sm.ch || sm.curr == IN_VAR)
    {
        if (sm.curr == DEFAULT)
        {
            if (flags & CONSUME_QUOTES && sm.ch == '\'')
               sm_trasition(&sm, IN_SQ);
            else if (flags & CONSUME_QUOTES && sm.ch == '"')
                sm_trasition(&sm, IN_DQ);
            else if (flags & EXPAND_VARS && sm.ch == '$')
                sm_trasition(&sm, IN_VAR);
            else
                sm_consume(&sm);
        }
        else if (sm.curr == IN_DQ)
        {
            if (flags & EXPAND_VARS && sm.ch == '$')
                sm_trasition(&sm, IN_VAR);
            else if (sm.ch == '"')
                sm_trasition(&sm, DEFAULT);
            else
                sm_consume(&sm);
        }
        else if (sm.curr == IN_SQ)
        {
            if (sm.ch == '\'')
                sm_trasition(&sm, DEFAULT);
            else
                sm_consume(&sm);
        }
        else if (sm.curr == IN_VAR)
        {
            if (is_valid(sm.ch) || sm.ch == '?')
            {
                var = envp_getvar_value(sh, &sm.str[sm.str_pos]);
                sm_cat(&sm, var);
                if (var)
                    free(var);
                while (is_valid(sm.ch) || sm.ch == '?')
                    sm_advance(&sm);
                sm_laststate(&sm);
            }
            else if ((sm.ch == '"' && sm.prev != IN_DQ)
                || (sm.ch == '\'' && sm.prev != IN_SQ))
                sm_laststate(&sm);
            else
            {
                sm_cat(&sm, "$");
                sm_laststate(&sm);
            }
        }
    }
    return (ft_strdup(sm.buffer));
}

char *expanded_gambiarra(t_envp *env, const char *str, int flags)
{
    t_minishell gambiarra;

    gambiarra.env = env;
    return (expanded(&gambiarra, str, flags));
}