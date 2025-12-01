#ifndef MINISHELL_SUBST_MACHINE_H
#define MINISHELL_SUBST_MACHINE_H

#include <stddef.h>
#include "globbing.h"
#include "utils.h"

typedef enum s_sm_state
{
    DEFAULT = 0,
    IN_DQ,
    IN_SQ,
    IN_VAR
} t_sm_state ;

typedef struct s_quote_machine
{
    t_sm_state     curr;
    t_sm_state     prev;
    const char  *str;
    size_t      str_pos;
    size_t      str_len;
    char        ch;
    char        buffer[ARG_MAX];
    size_t      buff_pos;
} t_quote_machine;

void    sm_init(t_quote_machine *sm, const char *str);
void    sm_advance(t_quote_machine *sm);
void    sm_trasition(t_quote_machine *sm, t_sm_state new);
void    sm_laststate(t_quote_machine *sm);
void    sm_consume(t_quote_machine *sm);
void    sm_cat(t_quote_machine *sm, const char *str);

#endif // MINISHELL_SUBST_MACHINE_H

