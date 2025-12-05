//
// Created by wlucas-f on 10/28/25.
//

#include <stdlib.h>
#include "../../includes/envp.h"
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"
#include "../../includes/globbing.h"
#include "../../includes/sm.h"

static void handle_quote_state(t_quote_machine *sm, int flags)
{
    char quote_char;

    if (sm->curr == IN_SQ)
        quote_char = '\'';
    else
        quote_char = '"';
    if (sm->curr == IN_DQ && sm->ch == '\\')
    {
        sm_advance(sm);
        if (sm->ch == '$' || sm->ch == '"' || sm->ch == '\\' || sm->ch == '`')
            sm_consume(sm);
        else
        {
            sm_cat(sm, "\\");
            sm_consume(sm);
        }
    }
    else if (sm->curr == IN_DQ && flags & EXPAND_VARS && sm->ch == '$')
        sm_trasition(sm, IN_VAR);
    else if (sm->ch == quote_char)
        sm_trasition(sm, DEFAULT);
    else
        sm_consume(sm);
}

static void handle_variable_state(t_quote_machine *sm, t_envp *env, int last_status)
{
    char *var;

    if (is_valid(sm->ch) || sm->ch == '?')
    {
        var = envp_getvar_value(env, &sm->str[sm->str_pos], last_status);
        sm_cat(sm, var);
        free(var);
        if (sm->ch == '?')
            sm_advance(sm);
        else
            while (is_valid(sm->ch))
                sm_advance(sm);
        sm_laststate(sm);
    }
    else if ((sm->ch == '"' && sm->prev != IN_DQ)
        || (sm->ch == '\'' && sm->prev != IN_SQ))
        sm_laststate(sm);
    else
    {
        sm_cat(sm, "$");
        sm_laststate(sm);
    }
}

static void	sm_run(t_quote_machine *sm, t_envp *env, int last_status, int flags)
{
    while (sm->ch || sm->curr == IN_VAR)
    {
        if (sm->curr == DEFAULT)
        {
            if (sm->ch == '\\')
            {
                sm_advance(sm);
                sm_consume(sm);
            }
            else if (flags & CONSUME_QUOTES && sm->ch == '\'')
                sm_trasition(sm, IN_SQ);
            else if (flags & CONSUME_QUOTES && sm->ch == '"')
                sm_trasition(sm, IN_DQ);
            else if (flags & EXPAND_VARS && sm->ch == '$')
                sm_trasition(sm, IN_VAR);
            else
                sm_consume(sm);
        }
        else if (sm->curr == IN_DQ || sm->curr == IN_SQ)
            handle_quote_state(sm, flags);
        else if (sm->curr == IN_VAR)
            handle_variable_state(sm, env, last_status);
    }
}

t_word *expanded(t_envp *env, const char *str, int last_status, int flags)
{
    t_quote_machine sm;
    t_word *result;

    sm_init(&sm, str);
    sm_run(&sm, env, last_status, flags);
    if (!*sm.buffer && sm.prev != IN_DQ && sm.prev != IN_SQ)
        return (NULL);
    result = malloc(sizeof(t_word));
    if (!result)
        return (NULL);
    result->content = ft_strdup(sm.buffer);
    result->len = sm.buff_pos + ((sm.prev == IN_DQ || sm.prev == IN_SQ) && !sm.buff_pos);
    result->quoted_map = malloc(sizeof(bool) * (result->len + 1));
    if (!result->quoted_map)
        return (free(result->content), free(result), NULL);
    ft_memcpy(result->quoted_map, sm.quoted_map, result->len * sizeof(bool));
    result->quoted_map[result->len] = false;
    return (result);
}

char *expanded_cstr(t_envp *env, const char *str, int last_status, int flags)
{
    t_quote_machine sm;

    sm_init(&sm, str);
    sm_run(&sm, env, last_status, flags);
    if (!*sm.buffer && sm.prev != IN_DQ && sm.prev != IN_SQ)
        return (NULL);
    return (ft_strdup(sm.buffer));
}
