#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <readline/readline.h>
#include    <readline/history.h>
#include "../includes/sig.h"
#include "../includes/tokens.h"
#include "../includes/parser.h"
#include "../includes/ast.h"
#include "../includes/envp.h"
#include "../includes/executor.h"
#include "../includes/globbing.h"
#include "../lib/libft/libft.h"

int     exec_line(t_minishell *sh)
{
    sh->aborted_parse = false;
    token_stream_init(sh);
    parse(sh);
    add_history(sh->line);
    free(sh->line);
    token_stream_free(sh->ts);
    //print_ast(sh->ast, 0);
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
        sh->line = NULL;
    }
    minishell_free(sh);
    return (true);
}

static int notty_line(t_minishell *sh)
{
    ssize_t  rd;
    char     c;
    char     *cursor;
    char     *line;

    line = ft_calloc(ARG_MAX, sizeof(char));
    if (!line)
        return (0);
    cursor = line;
    while (true)
    {
        rd = read(0, &c, 1);
        if (rd < 0)
            return (free(line), perror("read:"), 0);
        if (rd == 0 || c == '\n')
        {
            if (cursor == line && rd == 0)
                return (free(line), 0);
            *cursor = '\0';
            sh->line = line;
            return (1);
        }
        *cursor++ = c;
    }
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
    sh.env = &env;
    sh.ctx = &ctx;
    for (i = 0; envp[i] != NULL; i++)
        envp_setvar_str(sh.env, envp[i], sh.last_status, EXPORT);
    if (argc > 1)
    {
        sh.line = ft_strdup(argv[1]);
        exec_line(&sh);
        minishell_free(&sh);
        return (sh.last_status);
    }
    i = 0;
    if (!isatty(STDIN_FILENO))
    {
        while (notty_line(&sh))
        {
            if (sh.line && *sh.line != '\0')
                exec_line(&sh);
            sh.line = NULL;
        }
        minishell_free(&sh);
        return (sh.last_status);
    }
    rl_loop(&sh);
    return (sh.last_status);
}
