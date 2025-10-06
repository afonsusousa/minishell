//
// Created by afonsusousa on 9/24/25.
//

#ifndef MINISHELL_LEXER_H
#define MINISHELL_LEXER_H

#include <stddef.h>
#include <stdbool.h>

typedef enum e_token_type
{
    TOK_WORD = 0,
    TOK_QWORD,
    TOK_ASSIGNMENT_WORD,
    TOK_PIPE,         // |
    TOK_AND_IF,       // &&
    TOK_OR_IF,        // ||
    TOK_SEMI,         // ;
    TOK_AMP,          // &
    TOK_REDIR_IN,     // <
    TOK_REDIR_OUT,    // >
    TOK_REDIR_APPEND, // >>
    TOK_HEREDOC,      // <<
    TOK_LPAREN,       // (
    TOK_RPAREN,       // )
    TOK_STAR,         // *
    TOK_EOF
} t_token_type;

typedef struct s_token
{
    t_token_type type;
    const char  *lexeme;
    size_t       len;
}   t_token;

typedef struct s_lexer
{
    const char *input;
    size_t      input_len;
    size_t      position;
    size_t      read_position;
    char        ch;
} t_lexer;

// Lexer API
void lexer_read_char(t_lexer *lexer);
char lexer_peek_char(const t_lexer *lexer);
void lexer_read_word(t_lexer *lexer, t_token *token);
void lexer_skip_space(t_lexer *lexer);
bool token_strcmp(const t_token *token, const char *str);
t_token *token_new(const t_token_type type);
bool lexer_next_token_dmeta(t_lexer *lexer, t_token **token);
bool lexer_next_token_smeta(const t_lexer *lexer, t_token **token);
t_token *lexer_next_token(t_lexer *lexer);

#endif // MINISHELL_LEXER_H
