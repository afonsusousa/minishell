//
// Created by afonsusousa on 9/27/25.
//

#ifndef MINISHELL_MINISHELL_H
#define MINISHELL_MINISHELL_H
#include <sys/types.h>
#include "ast.h"
#include "tokens.h"
#include "envp.h"
#include "globbing.h"

typedef struct s_word t_word;

typedef struct s_pipeline
{
    pid_t   pids[1024];
    size_t  count;
    int     prev_read;
} t_pipeline;

typedef struct s_heredoc
{
    int             fd[2];
    t_word          *del;
    pid_t           pid;
    int             status;
}   t_heredoc;

typedef struct s_minishell
{
   t_envp         *env;
   t_envp         *ctx;
   char           *line;
   t_token_stream *ts;
   t_ast          *ast;
   bool            aborted_parse;
   t_pipeline     pipeline;
   t_heredoc      heredoc;
   int            last_status;
} t_minishell;

void    minishell_free(t_minishell *sh);

#endif //MINISHELL_MINISHELL_H