//
// Created by wlucas-f on 10/28/25.
//

#include <stdlib.h>
#include "../../includes/envp.h"
#include "../../includes/utils.h"
#include "../../lib/libft/libft.h"
#include "../../includes/globbing.h"
#include "../../includes/sm.h"

static void handle_default_state(t_quote_machine *sm, int flags)
{
    if (flags & CONSUME_QUOTES && sm->ch == '\'')
        sm_trasition(sm, IN_SQ);
    else if (flags & CONSUME_QUOTES && sm->ch == '"')
        sm_trasition(sm, IN_DQ);
    else if (flags & EXPAND_VARS && sm->ch == '$')
        sm_trasition(sm, IN_VAR);
    else
        sm_consume(sm);
}

static void handle_double_quote_state(t_quote_machine *sm, int flags)
{
    if (flags & EXPAND_VARS && sm->ch == '$')
        sm_trasition(sm, IN_VAR);
    else if (sm->ch == '"')
        sm_trasition(sm, DEFAULT);
    else
        sm_consume(sm);
}

static void handle_single_quote_state(t_quote_machine *sm)
{
    if (sm->ch == '\'')
        sm_trasition(sm, DEFAULT);
    else
        sm_consume(sm);
}

static void handle_variable_state(t_quote_machine *sm, const t_minishell *sh)
{
    char *var;

    if (is_valid(sm->ch) || sm->ch == '?')
    {
        var = envp_getvar_value(sh, &sm->str[sm->str_pos]);
        sm_cat(sm, var);
        if (var)
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

//TODO: is_valid needs fixing (allows much more characters!!)
char *expanded(const t_minishell *sh, const char *str, int flags)
{
    t_quote_machine sm;

    sm_init(&sm, str);
    while (sm.ch || sm.curr == IN_VAR)
    {
        if (sm.curr == DEFAULT)
            handle_default_state(&sm, flags);
        else if (sm.curr == IN_DQ)
            handle_double_quote_state(&sm, flags);
        else if (sm.curr == IN_SQ)
            handle_single_quote_state(&sm);
        else if (sm.curr == IN_VAR)
            handle_variable_state(&sm, sh);
    }
    return (ft_strdup(sm.buffer));
}

char *expanded_gambiarra(t_envp *env, const char *str, int flags)
{
    t_minishell gambiarra;

    gambiarra.env = env;
    return (expanded(&gambiarra, str, flags));
}