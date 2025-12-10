//
// Created by afonsusousa on 10/10/25.
//

#include "../includes/envp.h"
#include "../includes/minishell.h"
#include <readline/readline.h>

void	minishell_free(t_minishell *sh)
{
	rl_clear_history();
	ast_free(sh->ast);
	free_envp(sh->env);
	token_stream_free(sh->ts);
}