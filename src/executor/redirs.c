//
// Created by afonsusousa on 11/4/25.
//static

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "../../includes/minishell.h"
#include <fcntl.h>

#include "libft.h"
#include "utils.h"
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

static int handle_heredoc_redir(const t_ast *node)
{
    int fd;

    fd = node->as.redir.target.heredoc[0];
    if (fd < 0)
        return (0);
    if (dup2(fd, get_redir_fd(node->as.redir.kind)) < 0)
    {
        print_dup2_error();
        close(fd);
        return (1);
    }
    close(fd);
    return (0);
}

static int handle_file_redir(t_minishell *sh, const t_ast *node)
{
    int fd;
    char **filename;

    filename = expand_argv_word(sh, (char *)node->as.redir.target.file_name);
    if (filename && *filename && filename[1] != NULL)
    {
        write(2, "minishell: ", 11);
        write(2, node->as.redir.target.file_name, ft_strlen(node->as.redir.target.file_name));
        write(2, ": ambiguous redirect\n", 21);
        return (1);
    }
    if (!filename)
        return (0);
    fd = open_redir_file(node->as.redir.kind, *filename);
    if (fd < 0)
    {
        write(2, "minishell: ", 11);
        return (perror(*filename), 1);
    }
    if (dup2(fd, get_redir_fd(node->as.redir.kind)) < 0)
    {
        print_dup2_error();
        close(fd);
        return (1);
    }
    close(fd);
    return (0);
}

int exec_redirs(t_minishell* sh, const t_ast_list* r)
{
    int result;

    memset(&sh->heredoc, 0, sizeof(t_heredoc));
    while (r)
    {
        if (r->node->as.redir.kind == TOK_HEREDOC)
            result = handle_heredoc_redir(r->node);
        else
            result = handle_file_redir(sh, r->node);
        if (result != 0)
            return (1);
        r = r->next;
    }
    return (0);
}