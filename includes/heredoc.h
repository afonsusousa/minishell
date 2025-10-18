//
// Created by afonsusousa on 10/15/25.
//

#ifndef MINISHELL_HEREDOC_H
#define MINISHELL_HEREDOC_H

typedef struct s_heredoc
{
    pid_t   pids[256];
    size_t  count;
}   t_heredoc;
#endif //MINISHELL_HEREDOC_H