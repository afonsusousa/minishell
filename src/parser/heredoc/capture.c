//
// Created by afonsusousa on 10/14/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <signal.h>
#include "../../../includes/globbing.h"
#include "../../../lib/libft/libft.h"
#include <sys/wait.h>
#include <sys/types.h>
#include "minishell.h"
#include "sig.h"
#include "sm.h"
#include "utils.h"

static bool has_quotes(const t_word *delimiter)
{
    size_t i;

    if (!delimiter || !delimiter->quoted_map)
        return (false);
    i = 0;
    while (i < delimiter->len)
    {
        if (delimiter->quoted_map[i])
            return (true);
        i++;
    }
    return (false);
}

// TODO: checkout assign heredoc interplay
static char *expand_heredoc_line(t_minishell *sh, char **line, bool expand)
{
    t_word *tracked;
    char *ret;

    if (!expand || !*line)
        return (*line);
    tracked = expanded(sh->env, *line, sh->last_status, EXPAND_VARS);
    if (!tracked)
        return (*line);
    ret = ft_strdup(tracked->content);
    word_free(tracked);
    free(*line);
    *line = ret;
    return (ret);
}

static void run_heredoc_child(t_minishell *sh)
{
    char    *line;
    int     write_fd;
    bool    should_expand;

    write_fd = sh->heredoc.fd[1];
    signal(SIGINT, SIG_DFL);
    should_expand = !has_quotes(sh->heredoc.del);
    while (true)
    {
        line = readline("heredoc> ");
        if (ft_strcmp(line, sh->heredoc.del->content) == 0)
        {
            free(line);
            break ;
        }
        expand_heredoc_line(sh, &line, should_expand);
        if (!line)
            break ;
        write(write_fd, line, ft_strlen(line));
        write(write_fd, "\n", 1);
        free(line);
    }
    close(write_fd);
    word_free(sh->heredoc.del);
    exit(0);
}

static int handle_heredoc_parent(t_minishell *sh, int heredoc[2])
{
    close(sh->heredoc.fd[1]);
    signal(SIGINT, SIG_IGN);
    waitpid(sh->heredoc.pid, &sh->heredoc.status, 0);
    signal(SIGINT, sigint_handler);
    word_free(sh->heredoc.del);
    if (WIFSIGNALED(sh->heredoc.status) && WTERMSIG(sh->heredoc.status) == SIGINT)
    {
        write(1, "\n", 1);
        close(sh->heredoc.fd[0]);
        heredoc[0] = -1;
        heredoc[1] = -1;
        return (130);
    }
    heredoc[0] = sh->heredoc.fd[0];
    return (0);
}

int heredoc_setup(t_minishell *sh, int heredoc[2])
{
    if (!sh->heredoc.del)
        return (1);
    if (pipe(sh->heredoc.fd) != 0)
    {
        word_free(sh->heredoc.del);
        return (1);
    }
    sh->heredoc.pid = fork();
    if (sh->heredoc.pid < 0)
    {
        close(sh->heredoc.fd[0]);
        close(sh->heredoc.fd[1]);
        word_free(sh->heredoc.del);
        return (1);
    }
    if (sh->heredoc.pid == 0)
    {
        close(sh->heredoc.fd[0]);
        run_heredoc_child(sh);
    }
    return (handle_heredoc_parent(sh, heredoc));
}
