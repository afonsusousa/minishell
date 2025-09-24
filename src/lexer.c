/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 03:29:15 by amagno-r          #+#    #+#             */
/*   Updated: 2025/09/24 05:04:52 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum e_token_type
{
    TOK_WORD = 0,
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

//NEEDS TO ALLOW MORE CHARS
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

bool token_strcmp(t_token *token, const char *str)
{
    int i;

    i = 0;
    while (token->lexeme[i] == str[i] &&  i < token->len)
        i++;
    if (i == token->len)
        return (true);
    return (token->lexeme[i] - str[i]);
}

t_token   *token_new(t_token_type type)
{
    t_token *token;

    token = calloc(1, sizeof(t_token));
    if (token == NULL)
        return (NULL);
    token->type = type;
    return (token);
}
bool    *lexer_next_token_peek(t_lexer *lexer, t_token **token)
{
    char peek;

    peek = lexer_peek_char(lexer);
    if (lexer->ch == '|' && peek == '|')
        token = token_new(TOK_OR_IF);
    else if (lexer->ch == '&' && peek == '&')
        token = token_new(TOK_AND_IF);
    else if (lexer->ch == '>' && peek == '>')
        token = token_new(TOK_REDIR_APPEND);
    else if (lexer->ch == '<' && peek == '<')
        token = token_new(TOK_HEREDOC);
    else 
        return (false);
    return (true);
}
t_token *lexer_next_token(t_lexer *lexer)
{
    t_token *token;

    lexer_skip_space(lexer);
    if (lexer->ch == '\0')
        token = token_new(TOK_EOF);
    else if (lexer_next_token_peek(lexer, &token))
        lexer_read_char(lexer);
    else if (lexer->ch == '|')
        token = token_new(TOK_PIPE);
    else if (lexer->ch == '&')
        token = token_new(TOK_AMP);
    else if (lexer->ch == '>')
        token = token_new(TOK_REDIR_OUT);
    else if (lexer->ch == '<')
        token = token_new(TOK_REDIR_IN);
    else if (lexer->ch == ';')
        token = token_new(TOK_SEMI);
    else if (lexer->ch == '(')
        token = token_new(TOK_LPAREN);
    else if (lexer->ch == ')')
        token = token_new(TOK_RPAREN);
    else if (lexer->ch == '*')
        token = token_new(TOK_STAR);
    lexer_read_char(lexer);
    return (token);
}



