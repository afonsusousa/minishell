//
// Created by afonsusousa on 12/5/25.
//

#include "../../../includes/ast.h"
#include "../../../includes/minishell.h"
#include "../../../includes/parser.h"


static int redir_leaf(t_minishell *sh, const t_ast_list *redir)
{
    int status;

    status = 0;
    while (redir)
    {
        if (redir->node->as.redir.kind == TOK_HEREDOC)
        {
            sh->heredoc.del = expanded(sh->env,
                    redir->node->as.redir.target.file_name,
                    sh->last_status, CONSUME_QUOTES);
            status = heredoc_setup(sh, redir->node->as.redir.target.heredoc);
            if (status)
                return (status);
        }
        redir = redir->next;
    }
    return (0);
}

static int descend_pipeline(t_minishell *sh, const t_ast_list *cores)
{
    int status;

    while (cores)
    {
        status = heredoc_descend(sh, cores->node);
        if (status)
            return (status);
        cores = cores->next;
    }
    return (0);
}

int heredoc_descend(t_minishell *sh, t_ast* node)
{
    int status;

    status = 0;
    if (!node)
        return (0);
    if (node->type == AST_COMMAND_LINE)
        return (heredoc_descend(sh, node->as.command_line.list));
    if (node->type == AST_PIPELINE)
        return (descend_pipeline(sh, node->as.pipeline.cores));
    if (node->type == AST_COMMAND)
       return (redir_leaf(sh, node->as.command.redirs));
    if (node->type == AST_GROUPING)
    {
        status = heredoc_descend(sh, node->as.grouping.list);
        if (status)
            return (status);
        return (redir_leaf(sh, node->as.grouping.redirs));
    }
    if (node->type == AST_AND_LIST || node->type == AST_OR_LIST)
    {
        status = heredoc_descend(sh, node->as.binop.left);
        if (heredoc_descend(sh, node->as.binop.left))
            return (status);
        return (heredoc_descend(sh, node->as.binop.right));
    }
    return (0);
}