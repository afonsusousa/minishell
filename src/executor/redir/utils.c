//
// Created by afonsusousa on 12/3/25.
//

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../includes/minishell.h"
#include "../../../includes/executor.h"
#include "../../../includes/globbing.h"
#include "../../../includes/utils.h"
#include "../../../lib/libft/libft.h"

int get_redir_fd(const t_token_type r)
{
    if (r == TOK_REDIR_OUT || r == TOK_REDIR_APPEND)
        return (STDOUT_FILENO);
    return (STDIN_FILENO);
}

int handle_ambiguous_redirect(const char *filename, t_word **words)
{
    write(2, "minishell: ", 11);
    write(2, filename, ft_strlen(filename));
    write(2, ": ambiguous redirect\n", 21);
    word_free_until_null(words);
    return (1);
}

int handle_open_error(const char *filename, t_word **words)
{
    write(2, "minishell: ", 11);
    perror(filename);
    free((char *)filename);
    word_free_until_null(words);
    return (1);
}

int handle_dup2_error(int fd, const char *filename, t_word **words)
{
    print_dup2_error();
    if (fd > 2)
        close(fd);
    free((char *)filename);
    word_free_until_null(words);
    return (1);
}

