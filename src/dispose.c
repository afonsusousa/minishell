//
// Created by afonsusousa on 10/10/25.
//

#include <readline/readline.h>
#include "../includes/minishell.h"
#include "../includes/envp.h"

void    minishell_free(t_minishell *sh)
{
    rl_clear_history();
    ast_free(sh->ast);
    free_envp(sh->env);
    token_stream_free(sh->ts);
}