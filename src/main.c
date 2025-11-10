#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <readline/readline.h>

#include "../includes/lexer.h"
#include "../includes/tokens.h"
#include "../includes/parser.h"
#include "../includes/ast.h"
#include "../includes/envp.h"
#include "../includes/executor.h"
#include "../includes/subst.h"

static void print_indent(int depth)
{
    for (int i = 0; i < depth; ++i)
        fputs("  ", stdout);
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

static void print_ast(const t_ast *n, int d)
{
    if (!n) { print_indent(d); printf("(null)\n"); return; }
    switch (n->type) {
        case AST_COMMAND_LINE:
            print_indent(d); printf("COMMAND_LINE terminator=%c\n",
                n->as.command_line.terminator ? n->as.command_line.terminator : '0');
            print_ast(n->as.command_line.list, d + 1);
            break;
        case AST_OR_LIST:
            print_indent(d); printf("OR_LIST\n");
            print_ast(n->as.binop.left, d + 1);
            print_ast(n->as.binop.right, d + 1);
            break;
        case AST_AND_LIST:
            print_indent(d); printf("AND_LIST\n");
            print_ast(n->as.binop.left, d + 1);
            print_ast(n->as.binop.right, d + 1);
            break;
        case AST_PIPELINE: {
            print_indent(d); printf("PIPELINE\n");
            const t_ast_list *it = n->as.pipeline.cores;
            for (; it; it = it->next)
                print_ast(it->node, d + 1);
            break; }
        case AST_COMMAND: {
            print_indent(d); printf("COMMAND\n");
            print_indent(d + 1); printf("ASSIGNMENTS:\n");
            for (auto a = n->as.command.assignments; a && *a; a++)
            {
                print_indent(d + 2); printf("[%s]\n", *a);
            }
            print_indent(d + 1); printf("WORDS:\n");
            for (auto w = n->as.command.words; w && *w; w++)
            {
                print_indent(d + 2); printf("[%s]\n", *w);
            }
            const t_ast_list *r = n->as.command.redirs;
            for (; r; r = r->next) {
                print_indent(d + 1); printf("REDIRS\n");
                print_ast(r->node, d + 2);
            }
            break; }
        case AST_GROUPING:
            print_indent(d); printf("GROUPING (\n");
            print_ast(n->as.grouping.list, d + 1);
            print_indent(d); printf(")\n");
            break;
        case AST_REDIR:
            print_indent(d);
            printf("REDIR kind=%s target=", redir_kind_name(n->as.redir.kind));
            if (n->as.redir.target) printf("\"%s\"", n->as.redir.target);
            else printf("<missing>");
            printf("\n");
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
    sh->ast = parse(sh->ts->data, sh->ts->count);
    print_ast(sh->ast, 0);
    exec_ast(sh);
    return (sh->last_status);
}

//may return something else
int     rl_loop(t_minishell *sh)
{
    sh->line = readline("minishell> ");
    while (sh->line != NULL)
    {
        printf("%s\n", expanded(sh->env, sh->line));
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
    memset(&sh, 0, sizeof(t_minishell)); // ts -> NULL
    sh.ts = &ts;
    t_envp env = {0};
    t_envp ctx = {0};
    for (int i = 0; envp[i] != NULL; i++)
        envp_append_var(&env, envp[i], true);
    sh.env = &env;
    sh.ctx = &ctx;
    if (argc > 1)
    {
        sh.line = argv[1]; // simplified
        exec_line(&sh);
        minishell_free(&sh);
        return sh.last_status;
    }
    rl_loop(&sh);
    return (sh.last_status);
}
