/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 03:29:15 by amagno-r          #+#    #+#             */
/*   Updated: 2025/09/24 21:41:31 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../includes/lexer.h"
#include "../includes/utils.h"

#include "libft.h"

void lexer_read_char(t_lexer *lexer)
{
    if (lexer->read_position >= lexer->input_len)
        lexer->ch = '\0';
    else
        lexer->ch = lexer->input[lexer->read_position];
    lexer->position = lexer->read_position;
    lexer->read_position++;
}

char	lexer_peek_char(const t_lexer *lexer)
{
    if (lexer->read_position >= lexer->input_len)
        return ('\0');
    return (lexer->input[lexer->read_position]);
}

static int	is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

static int	is_meta(char c)
{
    return (c == '|' || c == '&' || c == ';'
        || c == '(' || c == ')' || c == '<' || c == '>');
}

static int	is_break(char c)
{
    return (c == '\0' || is_space(c) || is_meta(c));
}

// static int	handle_escape(t_lexer *lx, int in_squote, int in_dquote)
// {
//     char next;
//
//     if (lx->ch != '\\')
//         return (0);
//     if (in_squote)
//         return (0);
//     next = lexer_peek_char(lx);
//     if (in_dquote)
//     {
//         if (next == '$' || next == '`' || next == '"' || next == '\\' || next == '\n')
//         {
//             lexer_read_char(lx);
//             if (lx->ch != '\0')
//                 lexer_read_char(lx);
//             return (1);
//         }
//         return (0);
//     }
//     lexer_read_char(lx);
//     if (lx->ch != '\0')
//         lexer_read_char(lx);
//     return (1);
// }

static int	handle_quotes(t_lexer *lx, int *sq, int *dq)
{
    if (*dq == 0 && lx->ch == '\'')
    {
        *sq = !*sq;
        lexer_read_char(lx);
        return (1);
    }
    if (*sq == 0 && lx->ch == '"')
    {
        *dq = !*dq;
        lexer_read_char(lx);
        return (1);
    }
    return (0);
}

static t_token_type			evaluate_assign(const t_token *t)
{
    size_t i;
    size_t len;
    bool    first;

    i = 0;
    first = true;
    len = ft_strlen(t->lexeme);
    while (i < len)
    {
        if (first && t->lexeme[i] == '=')
        {
            if (i && i + 1 < len
                && ((!is_space(t->lexeme[i - 1]) || t->lexeme[i - 1] == '+')
                || !is_space(t->lexeme[i + 1])))
                return (t->lexeme[i - 1] == '+' ? TOK_APPEND_WORD : TOK_ASSIGNMENT_WORD);
            first = false;
        }
        if (!is_valid(t->lexeme[i]) && t->lexeme[i] != '+')
            return (t->type);
        i++;
    }
    return (t->type);
}

//TODO: better quote handling
void	lexer_read_word(t_lexer *lexer, t_token *token)
{
    size_t	start;
    int		sq;
    int		dq;

    start = lexer->position;
    sq = 0;
    dq = 0;
    while (lexer->ch != '\0')
    {
        if (handle_quotes(lexer, &sq, &dq))
            continue ;
        if (!sq && !dq && is_break(lexer->ch))
            break ;
        lexer_read_char(lexer);
    }
    token->type = TOK_WORD;
    token->lexeme = ft_strndup(lexer->input + start,
                    lexer->position - start);
}

void lexer_skip_space(t_lexer *lexer)
{
    while (is_space(lexer->ch))
        lexer_read_char(lexer);
}

t_token   *token_new(const t_token_type type)
{
    t_token *token;

    token = calloc(1, sizeof(t_token));
    if (token == NULL)
        return (NULL);
    token->type = type;
    return (token);
}
bool    lexer_next_token_dmeta(t_lexer *lexer, t_token **token)
{
    char peek;

    peek = lexer_peek_char(lexer);
    if (lexer->ch == '|' && peek == '|')
        *token = token_new(TOK_OR_IF);
    else if (lexer->ch == '&' && peek == '&')
        *token = token_new(TOK_AND_IF);
    else if (lexer->ch == '>' && peek == '>')
        *token = token_new(TOK_REDIR_APPEND);
    else if (lexer->ch == '<' && peek == '<')
        *token = token_new(TOK_HEREDOC);
    else 
        return (false);
    lexer_read_char(lexer);
    return (true);
}

bool    lexer_next_token_smeta(const t_lexer *lexer, t_token **token)
{
    if (lexer->ch == '|')
        *token = token_new(TOK_PIPE);
    else if (lexer->ch == '&')
        *token = token_new(TOK_AMP);
    else if (lexer->ch == '>')
        *token = token_new(TOK_REDIR_OUT);
    else if (lexer->ch == '<')
        *token = token_new(TOK_REDIR_IN);
    else if (lexer->ch == ';')
        *token = token_new(TOK_SEMI);
    else if (lexer->ch == '(')
        *token = token_new(TOK_LPAREN);
    else if (lexer->ch == ')')
        *token = token_new(TOK_RPAREN);
    else
        return (false);
    return (true);
}

t_token *lexer_next_token(t_lexer *lexer)
{
    t_token	*token;

    lexer_skip_space(lexer);
    if (lexer->ch == '\0')
        token = token_new(TOK_EOF);
    else if (lexer_next_token_dmeta(lexer, &token))
        lexer_read_char(lexer);
    else if (lexer_next_token_smeta(lexer, &token))
        lexer_read_char(lexer);
    else
    {
        token = token_new(TOK_WORD);
        if (token == NULL)
            return (NULL);
        lexer_read_word(lexer, token);
        token->type = evaluate_assign(token);
    }
    return (token);
}