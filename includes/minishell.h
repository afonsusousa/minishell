//
// Created by afonsusousa on 9/27/25.
//

#ifndef MINISHELL_MINISHELL_H
#define MINISHELL_MINISHELL_H
#include <sys/types.h>
#include "ast.h"
#include "tokens.h"
#include "envp.h"

typedef struct s_pipeline
{
    int     pipefd[2];
    int     prev_read;
    pid_t   pids[256];
    size_t  count;
} t_pipeline;

typedef struct s_heredoc
{
    pid_t   pids[256];
    size_t  count;
}   t_heredoc;

typedef struct s_minishell
{
   t_envp         *env;
   t_envp         *ctx;
   char           *line;
   t_token_stream *ts;
   t_ast          *ast;
   t_pipeline     pipeline;
   t_heredoc      heredoc;
   int            last_status;
} t_minishell;

void    minishell_free(t_minishell *sh);

#endif //MINISHELL_MINISHELL_H