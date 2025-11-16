#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <readline/readline.h>
#include "../includes/lexer.h"
#include "../includes/sig.h"
#include "../includes/tokens.h"
#include "../includes/parser.h"
#include "../includes/ast.h"
#include "../includes/envp.h"
#include "../includes/executor.h"
#include "../includes/globbing.h"
#include <signal.h>
#include <termios.h>

volatile int sig = 1;

static void print_indent(int depth)
{
    for (int i = 0; i < depth; ++i)
        fputs("    ", stdout);
}

static const char *redir_kind_name(t_token_type k)
{
    switch (k) {
        case TOK_REDIR_IN: return "<";
        case TOK_REDIR_OUT: return ">";
        case TOK_REDIR_APPEND: return ">>";
        case TOK_HEREDOC: return "<<";
        default: return "?";
    }
}

static void print_strs(const char **strs)
{
    const char **s;

    s = strs;
    if (!strs)
    {
        printf("[N/A]\n");
        return ;
    }
    while (*s != NULL)
    {
        printf("[\"%s\"]", *s);
        if (s[1])
            printf(" ");
        s++;
    }
   printf("\n");
}

static void print_ast(const t_ast *n, int d)
{
    if (!n) { print_indent(d); printf("(null)\n"); return; }
    switch (n->type) {
        case AST_COMMAND_LINE:
            print_indent(d); printf("[command_line] terminator=%c\n",
                n->as.command_line.terminator ? n->as.command_line.terminator : '0');
            print_ast(n->as.command_line.list, d + 1);
            break;
        case AST_OR_LIST:
            print_indent(d); printf("[or_list]\n");
            print_ast(n->as.binop.left, d + 1);
            print_ast(n->as.binop.right, d + 1);
            break;
        case AST_AND_LIST:
            print_indent(d); printf("[and_list]\n");
            print_ast(n->as.binop.left, d + 1);
            print_ast(n->as.binop.right, d + 1);
            break;
        case AST_PIPELINE: {
            print_indent(d); printf("[pipeline]\n");
            const t_ast_list *it = n->as.pipeline.cores;
            for (; it; it = it->next)
                print_ast(it->node, d + 1);
            break; }
        case AST_COMMAND: {
            print_indent(d); printf("[command]\n");
            print_indent(d + 1); printf("[assigments]\t");
            print_strs(n->as.command.assignments);
            print_indent(d + 1); printf("[argv]\t\t");
            print_strs(n->as.command.argv);
            const t_ast_list *r = n->as.command.redirs;
            print_indent(d + 1); printf("[redirs]\t\t");
            if (!r)
                printf("[N/A]");
            for (; r; r = r->next) {
                print_ast(r->node, d + 2);
                if (r->next)
                    printf(" ");
            }
            printf("\n");
            break; }
        case AST_GROUPING:
            print_indent(d); printf("[grouping] \n");
            print_indent(d); printf("[\n");
            print_ast(n->as.grouping.list, d + 1);
            print_indent(d); printf("]\n");
            break;
        case AST_REDIR:
            printf("[%s,", redir_kind_name(n->as.redir.kind));
            if (n->as.redir.target.file_name
                && n->as.redir.kind != TOK_HEREDOC)
                printf("\"%s\"]", n->as.redir.target.file_name);
            else printf("<missing>");
            break;
        default:
            print_indent(d); printf("<unknown node %d>\n", (int)n->type);
            break;
    }
}

void    init_lexer(t_lexer *lx, char *input)
{
    lx->input = input;
    lx->input_len = strlen(input);
    lx->position = 0;
    lx->read_position = 0;
    lx->ch = '\0';
    lexer_read_char(lx);
}

char    **get_matches(char *cwd, char **wildstr);

int     exec_line(t_minishell *sh)
{
    t_lexer lx;
    init_lexer(&lx, sh->line);
    token_stream_init(sh, 0);
    token_stream_fill(sh->ts, &lx);
    parse(sh);
    print_ast(sh->ast, 0);
    exec_ast(sh);
    return (sh->last_status);
}

int     rl_loop(t_minishell *sh)
{
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    sh->line = readline("minishell> ");
    while (sh->line != NULL)
    {
        printf("%s\n", expanded(sh->env, sh->line, true));
        exec_line(sh);
        free(sh->line);
        token_stream_free(sh->ts);
        ast_free(sh->ast);
        sh->ast = NULL;
        sh->line = readline("minishell> ");
    }
    minishell_free(sh);
    return (true);
}

int main(int argc, char **argv, char **envp)
{
    t_minishell sh;
    t_token_stream ts;
    printf("%d\n", getpid());
    memset(&sh, 0, sizeof(t_minishell));
    sh.ts = &ts;
    t_envp env = {0};
    t_envp ctx = {0};
    for (int i = 0; envp[i] != NULL; i++)
        envp_append_var(&env, envp[i], true);
    sh.env = &env;
    sh.ctx = &ctx;
    if (argc > 1)
    {
        sh.line = argv[1];
        exec_line(&sh);
        minishell_free(&sh);
        return sh.last_status;
    }
    rl_loop(&sh);
    return (sh.last_status);
}
