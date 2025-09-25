#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../includes/lexer.h"
#include "../includes/tokens.h"

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

int main(int argc, char **argv)
{
    const char *input = NULL;
    if (argc > 1)
        input = argv[1];
    else
        input = "a=\"pila\" echo 'hello' && ls | wc -l";
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
    for (size_t i = 0; i < ts.count; ++i) {
        const t_token *t = &ts.data[i];
        printf("%-14s | %.*s\n", tok_name(t->type), (int)t->len, t->lexeme);
        if (t->type == TOK_EOF) break;
    }
    token_stream_free(&ts);
    return 0;
}

