//
// Created by afonsusousa on 11/4/25.
//static

#include <string.h>
#include <unistd.h>
#include "../../includes/minishell.h"
#include <fcntl.h>
#include "../../includes/globbing.h"

int get_redir_fd(const t_token_type r)
{
    if (r == TOK_REDIR_OUT || r == TOK_REDIR_APPEND)
        return (STDOUT_FILENO);
    return (STDIN_FILENO);
}

int exec_redirs(t_minishell* sh, t_ast_list* r, bool duplicate)
{
    int fd;
    char* filename;
    memset(&sh->heredoc, 0, sizeof(t_heredoc));
    while (r)
    {
        filename = (char *) r->node->as.redir.target.file_name;
        //subst
        if (r->node->as.redir.kind == TOK_REDIR_OUT)
            fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (r->node->as.redir.kind == TOK_REDIR_IN)
            fd = open(filename, O_RDONLY);
        else if (r->node->as.redir.kind == TOK_REDIR_APPEND)
            fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        //aqui eu vou me foder
        else if (r->node->as.redir.kind == TOK_HEREDOC)
            fd = r->node->as.redir.target.heredoc[0];
        else
            fd = -1;
        if (duplicate && dup2(fd, get_redir_fd(r->node->as.redir.kind)) < 0)
        {
            close(fd);
            return (1);
        }
        if (fd < 0)
        {
            //error
            return (1);
        }
        close(fd);
        r = r->next;
    }
    return (0);
}