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
    while (!sh->aborted_parse && is_redir_ahead(sh->ts))
    {
        redir_node = parse_redir(sh);
        if (!redir_node)
            break;
        if (ast_list_push(&redirs, redir_node) == NULL)
            break;
    }
    return (redirs);
}

t_ast *parse_redir(t_minishell *sh)
{
    t_ast		*redir;
    t_token_type redir_type;

    if (sh->aborted_parse)
        return (NULL);
    redir_type = is_redir_token_type(sh->ts);
    if (!redir_type)
        return (NULL);
    redir = ast_make_redir_node(redir_type);
    if (!redir)
        return (NULL);
    if (redir_type == TOK_HEREDOC && ts_match(sh->ts, TOK_WORD))
    {
        heredoc_setup(sh, redir->as.redir.target.heredoc);
        if (sh->aborted_parse || redir->as.redir.target.heredoc[1] == -1)
            return (ast_free(redir), NULL);
    }
    else if (ts_match(sh->ts, TOK_WORD))
    {
        redir->as.redir.target.file_name = ft_strdup(sh->ts->tk->lexeme);
    }
    else
        return (parser_abort_error(sh), ast_free(redir), NULL);
    return (redir);
}