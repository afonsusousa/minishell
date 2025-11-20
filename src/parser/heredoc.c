//
// Created by afonsusousa on 10/14/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <signal.h>
#include "../../includes/globbing.h"
#include "../../lib/libft/libft.h"
#include <sys/wait.h>
#include <sys/types.h>
#include "minishell.h"
#include "sig.h"
#include "utils.h"

static int is_quoted(const char *delimiter)
{
    while (*delimiter)
    {
        if (*delimiter == '\'' || *delimiter == '"')
            return (1);
        delimiter++;
    }
    return (0);
}

static int  open_heredoc_pipe(int fd[2])
{
    if (pipe(fd) != 0)
        return (0);
    return (1);
}

static void run_heredoc_child(t_minishell *sh)
{
    char    *line;
    int     write_fd;

    write_fd = sh->heredoc.fd[1];
    signal(SIGINT, SIG_DFL);
    while (1)
    {
        line = readline("heredoc> ");
        if (!line)
            break;
        if (ft_strcmp(line, sh->heredoc.del) == 0)
        {
            free(line);
            break;
        }
        write(write_fd, line, ft_strlen(line));
        write(write_fd, "\n", 1);
        free(line);
    }
    if (sh->heredoc.quoted == 1)
        free(sh->heredoc.del);
    close(write_fd);
    if (sh->heredoc.quoted)
        free(sh->heredoc.del);
    minishell_free(sh);
    exit(0);
}

static void handle_heredoc_parent(t_minishell *sh, int heredoc[2])
{
    close(sh->heredoc.fd[1]);
    signal(SIGINT, SIG_IGN);
    waitpid(sh->heredoc.pid, &sh->heredoc.status, 0);
    signal(SIGINT, sigint_handler);
    if (sh->heredoc.quoted == 1)
        free(sh->heredoc.del);
    if (WIFSIGNALED(sh->heredoc.status) && WTERMSIG(sh->heredoc.status) == SIGINT)
    {
        write(1, "\n", 1);
        close(sh->heredoc.fd[0]);
        heredoc[0] = -1;
        heredoc[1] = -1;
        sh->aborted_parse = true;
        sh->last_status = 130;
    }
    else
        heredoc[0] = sh->heredoc.fd[0];
}

void heredoc_setup(t_minishell *sh, int heredoc[2])
{
    sh->heredoc.del = (char *) sh->ts->tk->lexeme;
    sh->heredoc.quoted = is_quoted(sh->heredoc.del);
    if (sh->heredoc.quoted)
        sh->heredoc.del = expanded(NULL, sh->heredoc.del, false, true);
    if (!open_heredoc_pipe(sh->heredoc.fd))
        return ;
    sh->heredoc.pid = fork();
    if (sh->heredoc.pid < 0)
    {
        close(sh->heredoc.fd[0]);
        close(sh->heredoc.fd[1]);
        if (sh->heredoc.quoted == 1)
            free(sh->heredoc.del);
        return;
    }
    if (sh->heredoc.pid == 0)
    {
        close(sh->heredoc.fd[0]);
        run_heredoc_child(sh);
    }
    handle_heredoc_parent(sh, heredoc);
}
