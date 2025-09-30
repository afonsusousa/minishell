#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../includes/lexer.h"
#include "../includes/tokens.h"
#include "../includes/parser.h"
#include "../includes/ast.h"
#include "../includes/envp.h"
#include "../includes/subst.h"

static const char *tok_name(t_token_type t)
{
    switch (t) {
        case TOK_WORD: return "WORD";
        case TOK_ASSIGNMENT_WORD: return "ASSIGN";
        case TOK_PIPE: return "PIPE";
        case TOK_AND_IF: return "AND_IF";
        case TOK_OR_IF: return "OR_IF";
        case TOK_SEMI: return "SEMI";
        case TOK_AMP: return "AMP";
        case TOK_REDIR_IN: return "REDIR_IN";
        case TOK_REDIR_OUT: return "REDIR_OUT";
        case TOK_REDIR_APPEND: return "REDIR_APPEND";
        case TOK_HEREDOC: return "HEREDOC";
        case TOK_LPAREN: return "LPAREN";
        case TOK_RPAREN: return "RPAREN";
        case TOK_STAR: return "STAR";
        case TOK_EOF: return "EOF";
        default: return "?";
    }
}

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

static void print_word_leaf(const t_ast *n)
{
    printf("\"%.*s\"", (int)n->as.leaf.len, n->as.leaf.text);
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
            const t_ast_list *it = n->as.pipeline.commands;
            for (; it; it = it->next)
                print_ast(it->node, d + 1);
            break; }
        case AST_COMMAND: {
            print_indent(d); printf("COMMAND\n");
            print_ast(n->as.command.core, d + 1);
            const t_ast_list *r = n->as.command.redirs;
            for (; r; r = r->next) {
                print_indent(d + 1); printf("TRAILING_REDIR\n");
                print_ast(r->node, d + 2);
            }
            break; }
        case AST_SIMPLE_COMMAND: {
            print_indent(d); printf("SIMPLE_COMMAND\n");
            const t_ast_list *a = n->as.simple_command.assignments;
            for (; a; a = a->next) {
                print_indent(d + 1); printf("ASSIGN ");
                print_word_leaf(a->node);
                printf("\n");
            }
            const t_ast_list *w = n->as.simple_command.words;
            for (; w; w = w->next) {
                print_indent(d + 1); printf("WORD ");
                print_word_leaf(w->node);
                printf("\n");
            }
            const t_ast_list *ir = n->as.simple_command.redirs;
            for (; ir; ir = ir->next) {
                print_indent(d + 1); printf("REDIR\n");
                print_ast(ir->node, d + 2);
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
            if (n->as.redir.target) print_word_leaf(n->as.redir.target);
            else printf("<missing>");
            printf("\n");
            break;
        case AST_ASSIGNMENT:
            print_indent(d); printf("ASSIGN_LEAF ");
            print_word_leaf(n);
            printf("\n");
            break;
        case AST_WORD:
            print_indent(d); printf("WORD_LEAF ");
            print_word_leaf(n);
            printf("\n");
            break;
        default:
            print_indent(d); printf("<unknown node %d>\n", (int)n->type);
            break;
    }
}

int main(int argc, char **argv, char **envp)
{
    const char *input = NULL;
    if (argc > 1)
        input = argv[1];
    else
        input = "a=\"pila\" echo 'hello' < out && ls | wc -l && (ls)";

    // Lex
    t_lexer lx;
    lx.input = input;
    lx.input_len = strlen(input);
    lx.position = 0;
    lx.read_position = 0;
    lx.ch = '\0';
    lexer_read_char(&lx); // prime first char
    t_token_stream ts;
    token_stream_init(&ts, 0);
    if (!token_stream_fill(&ts, &lx)) {
        fprintf(stderr, "lexing failed (OOM?)\n");
        token_stream_free(&ts);
        return 1;
    }

    // Optional: dump tokens
    printf("TOKENS:\n");
    for (size_t i = 0; i < ts.count; ++i) {
        const t_token *t = &ts.data[i];
        printf("%-14s | %.*s\n", tok_name(t->type), (int)t->len, t->lexeme);
        if (t->type == TOK_EOF) break;
    }

    // Parse
    t_ast *root = parse(ts.data, ts.count);
    if (!root) {
        fprintf(stderr, "parse failed\n");
        token_stream_free(&ts);
        return 2;
    }

    // Print AST
    printf("\nAST:\n");
    print_ast(root, 0);

    // Cleanup
    ast_free(root);
    token_stream_free(&ts);

    t_envp env = {0};
    for (int i = 0; envp[i] != NULL; i++)
        envp_elem_set(&env, envp[i]);
    printf ("%s\n", expanded_str(&env, "$USERNAME $ $ $ $USERNAME"));
    return 0;
}
