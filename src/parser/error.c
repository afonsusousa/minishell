//
// Created by afonsusousa on 11/18/25.
//

#include <unistd.h>

#include "../../includes/minishell.h"
#include "../../lib/libft/libft.h"

static const char   *get_operator_token_name(const t_token_type type)
{
    if (type == TOK_PIPE)
        return ("|");
    if (type == TOK_AND)
        return ("&&");
    if (type == TOK_OR)
        return ("||");
    if (type == TOK_SEMI)
        return (";");
    if (type == TOK_AMP)
        return ("&");
    return (NULL);
}

static const char   *get_redir_token_name(const t_token_type type)
{
    if (type == TOK_REDIR_IN)
        return ("<");
    if (type == TOK_REDIR_OUT)
        return (">");
    if (type == TOK_REDIR_APPEND)
        return (">>");
    if (type == TOK_HEREDOC)
        return ("<<");
    return (NULL);
}

static const char   *get_token_type_name(const t_token *tk)
{
    const char  *name;

    if (tk->type == TOK_WORD || tk->type == TOK_ASSIGNMENT_WORD)
        return (tk->lexeme);
    name = get_operator_token_name(tk->type);
    if (name)
        return (name);
    name = get_redir_token_name(tk->type);
    if (name)
        return (name);
    if (tk->type == TOK_LPAREN)
        return ("(");
    else if (tk->type == TOK_RPAREN)
        return (")");
    else if (tk->type == TOK_EOF)
        return ("newline");
    return ("unknown token");
}

static void print_syntax_error(const char *token_name)
{
    ft_putstr_fd("minishell: syntax error near unexpected token '", \
        STDERR_FILENO);
    ft_putstr_fd((char *)token_name, STDERR_FILENO);
    ft_putstr_fd("'\n", STDERR_FILENO);
}

void    get_token_name(const t_token *tk)
{
    const char  *name;

    name = get_token_type_name(tk);
    print_syntax_error(name);
}

void    parser_abort(t_minishell *sh)
{
    sh->aborted_parse = true;
}

void    parser_abort_error(t_minishell *sh)
{
    get_token_name(ts_peek(sh->ts));
    parser_abort(sh);
}