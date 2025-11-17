//
// Created by afonsusousa on 11/4/25.
//

#include "minishell.h"
#include "../../includes/parser.h"
#include "../../lib/libft/libft.h"

t_ast_list	*parse_core_redirs(t_minishell *sh)
{
    t_ast_list	*redirs;
    t_ast		*redir_node;

    redirs = NULL;
    while (is_redir_ahead(sh->ts))
    {
        redir_node = parse_redir(sh);
        if (redir_node == NULL)
            break;
        if (ast_list_push(&redirs, redir_node) == NULL)
            break;
    }
    return (redirs);
}

t_ast *parse_redir(t_minishell *sh)
{
    t_ast		*redir;
    const t_token *tk;

    tk = ts_peek(sh->ts);
    if (!tk || !is_redir_token_type(tk->type))
        return (NULL);
    ts_advance(sh->ts);
    redir = ast_make_redir_node(tk->type);
    if (!redir)
        return (NULL);
    if (tk->type == TOK_HEREDOC && ts_peek(sh->ts)->type == TOK_WORD)
    {
        ts_advance(sh->ts);
        heredoc_setup(sh, redir->as.redir.target.heredoc);
        if (redir->as.redir.target.heredoc[1] == -1)
            return (ast_free(redir), NULL);
    }
    else if ((ts_peek(sh->ts)->type == TOK_WORD))
    {
        ts_advance(sh->ts);
        redir->as.redir.target.file_name = ft_strdup(tk->lexeme);
    }
    else
        return (ast_free(redir), NULL);
    return (redir);
}