/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 03:29:15 by amagno-r          #+#    #+#             */
/*   Updated: 2025/09/24 04:11:10 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdbool.h>

typedef enum e_token_type {
    TOK_WORD = 0,
    TOK_ASSIGNMENT_WORD,
    TOK_PIPE,          // |
    TOK_AND_IF,        // &&
    TOK_OR_IF,         // ||
    TOK_SEMI,          // ;
    TOK_AMP,           // &
    TOK_REDIR_IN,      // <
    TOK_REDIR_OUT,     // >
    TOK_REDIR_APPEND,  // >>
    TOK_HEREDOC,       // <<
    TOK_LPAREN,        // (
    TOK_RPAREN,        // )
    TOK_STAR,          // *
    TOK_EOF
}   t_token_type;

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

void lexer_read_char(t_lexer *lexer)
{
    if (lexer->read_position >= lexer->input_len)
        lexer->ch = '\0';
    else
        lexer->ch = lexer->input[lexer->read_position];
    lexer->position = lexer->read_position;
    lexer->read_position++;
}

char lexer_peek_char(t_lexer *lexer)
{
    if (lexer->read_position >= lexer->input_len)
		lexer->ch = '\0';
	else
		lexer->ch = lexer->input[lexer->read_position];
  return lexer->ch;
}

void lexer_skip_space(t_lexer *lexer)
{
    while (lexer->ch == ' ' || lexer->ch == '\t' || lexer->ch == '\n')
        lexer_read_char(lexer);
}

void lexer_read_word(t_lexer *lexer, t_token *token)
{
    size_t  start_pos;
    
    start_pos = lexer->position;
    while ((lexer->ch >= 'A' && lexer->ch <= 'Z') 
            || (lexer->ch >= 'a' && lexer->ch <= 'z')
            || lexer->ch == '_' && lexer->ch == '-')
            lexer_read_char(lexer);
    token->lexeme = lexer->input + lexer->position;
    token->len = lexer->position - start_pos;
}

bool token_strcmp(t_token *token, char *str)
{
    int i;

    i = 0;
    while (token->lexeme[i] == str[i] &&  i < token->len)
        i++;
    if (i == token->len)
        return (true);
    return (token->lexeme[i] - str[i]);
}

void token_assign_type(t_token *token)
{
    
}



