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
#include <ctype.h>
#include "libft.h"

#include "../../includes/lexer.h"
#include "../../includes/utils.h"

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

//TODO: fully move append distinction to a saner place
static t_token_type			evaluate_assign(const t_token *t)
{
    size_t i;
    size_t len;
    bool    first;

    i = 0;
    first = true;
    len = ft_strlen(t->lexeme);
    if (isdigit(t->lexeme[0]))
        return (t->type);
    while (i < len)
    {
        if (first && t->lexeme[i] == '=')
        {
            if (i && i + 1 < len
                && ((!is_space(t->lexeme[i - 1]) || t->lexeme[i - 1] == '+')
                || !is_space(t->lexeme[i + 1])))
                return (TOK_ASSIGNMENT_WORD);
            first = false;
        }
        if (!is_valid(t->lexeme[i]) && t->lexeme[i] != '+')
            return (t->type);
        i++;
    }
    return (t->type);
}

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
bool    lexer_next_dmeta(t_lexer *lexer)
{
    char peek;

    peek = lexer_peek_char(lexer);
    if (lexer->ch == '|' && peek == '|')
        lexer->tk = token_new(TOK_OR);
    else if (lexer->ch == '&' && peek == '&')
        lexer->tk = token_new(TOK_AND);
    else if (lexer->ch == '>' && peek == '>')
        lexer->tk = token_new(TOK_REDIR_APPEND);
    else if (lexer->ch == '<' && peek == '<')
        lexer->tk = token_new(TOK_HEREDOC);
    else 
        return (false);
    lexer_read_char(lexer);
    return (true);
}

bool    lexer_next_smeta(t_lexer *lexer)
{
    if (lexer->ch == '|')
        lexer->tk = token_new(TOK_PIPE);
    else if (lexer->ch == '&')
        lexer->tk = token_new(TOK_AMP);
    else if (lexer->ch == '>')
        lexer->tk = token_new(TOK_REDIR_OUT);
    else if (lexer->ch == '<')
        lexer->tk = token_new(TOK_REDIR_IN);
    else if (lexer->ch == ';')
        lexer->tk  = token_new(TOK_SEMI);
    else if (lexer->ch == '(')
        lexer->tk = token_new(TOK_LPAREN);
    else if (lexer->ch == ')')
        lexer->tk = token_new(TOK_RPAREN);
    else
        return (false);
    return (true);
}

t_token *lexer_next_token(t_lexer *lexer)
{
    lexer_skip_space(lexer);
    if (lexer->ch == '\0')
        lexer->tk = token_new(TOK_EOF);
    else if (lexer_next_dmeta(lexer) || lexer_next_smeta(lexer))
        lexer_read_char(lexer);
    else
    {
        lexer->tk = token_new(TOK_WORD);
        if (lexer->tk == NULL)
            return (NULL);
        lexer_read_word(lexer, lexer->tk);
        lexer->tk->type = evaluate_assign(lexer->tk);
    }
    return (lexer->tk);
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