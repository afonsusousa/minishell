#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include "../includes/sig.h"
#include "../includes/tokens.h"
#include "../includes/parser.h"
#include "../includes/ast.h"
#include "../includes/envp.h"
#include "../includes/executor.h"
#include "../includes/globbing.h"

int     exec_line(t_minishell *sh)
{
    sh->aborted_parse = false;
    token_stream_init(sh);
    parse(sh);
    token_stream_free(sh->ts);
    if (sh->aborted_parse || !sh->ast)
        return (sh->last_status);
    exec_ast(sh);
    ast_free(sh->ast);
    sh->ast = NULL;
    return (sh->last_status);
}

int     rl_loop(t_minishell *sh)
{
    signal_setup();
    while (1)
    {
        sh->line = readline("minishell> ");
        if (sh->line == NULL)
            break;
        if (*sh->line != '\0')
            exec_line(sh);
        free(sh->line);
        sh->line = NULL;
    }
    minishell_free(sh);
    return (true);
}

int main(int argc, char **argv, char **envp)
{
    t_minishell sh;
    t_token_stream ts = {0};
    t_envp env = (t_envp){0};
    t_envp ctx = (t_envp){0};
    int i;

    signal_setup();
    memset(&sh, 0, sizeof(t_minishell));
    sh.ts = &ts;
    for (i = 0; envp[i] != NULL; i++)
        envp_append_var(&env, envp[i], true);
    sh.env = &env;
    sh.ctx = &ctx;
    if (argc > 1)
    {
        sh.line = argv[1];
        exec_line(&sh);
        minishell_free(&sh);
        return (sh.last_status);
    }
    rl_loop(&sh);
    return (sh.last_status);
}
