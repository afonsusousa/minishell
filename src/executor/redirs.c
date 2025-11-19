//
// Created by afonsusousa on 11/4/25.
//static

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "../../includes/minishell.h"
#include <fcntl.h>
#include "../../includes/globbing.h"
#include "../../includes/executor.h"

int get_redir_fd(const t_token_type r)
{
    if (r == TOK_REDIR_OUT || r == TOK_REDIR_APPEND)
        return (STDOUT_FILENO);
    return (STDIN_FILENO);
}

static int open_redir_file(const t_token_type kind, const char *filename)
{
    int fd;

    fd = -1;
    if (kind == TOK_REDIR_OUT)
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (kind == TOK_REDIR_IN)
        fd = open(filename, O_RDONLY);
    else if (kind == TOK_REDIR_APPEND)
        fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    return (fd);
}

static int apply_redir(const int fd, const t_token_type kind)
{
    if (dup2(fd, get_redir_fd(kind)) < 0)
    {
        print_dup2_error();
        close(fd);
        return (1);
    }
    return (0);
}

static int handle_redir_node(const t_ast *node, const bool duplicate)
{
    int fd;
    char *filename;

    filename = (char *)node->as.redir.target.file_name;
    if (node->as.redir.kind != TOK_HEREDOC)
    {
        fd = open_redir_file(node->as.redir.kind, filename);
        if (fd < 0)
            return (print_open_error(filename), 1);
    }
    else
        fd = node->as.redir.target.heredoc[0];
    if (fd >= 0 && duplicate && apply_redir(fd, node->as.redir.kind) != 0)
            return (1);
    if (fd >= 0)
        close(fd);
    return (0);
}

int exec_redirs(t_minishell* sh, const t_ast_list* r, bool duplicate)
{
    memset(&sh->heredoc, 0, sizeof(t_heredoc));
    while (r)
    {
        if (handle_redir_node(r->node, duplicate) != 0)
            return (1);
        r = r->next;
    }
    return (0);
}