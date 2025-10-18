//
// Created by afonsusousa on 9/27/25.
//

#ifndef MINISHELL_MINISHELL_H
#define MINISHELL_MINISHELL_H
#include <stddef.h>

#include "ast.h"
#include "tokens.h"
#include "pipeline.h"
#include "heredoc.h"
#include "../lib/libft/libft.h"

typedef struct s_envp_elem
{
   char     *str;
   size_t   tag_len;
} t_var;

typedef struct s_envp
{
   t_var       *vars;
   size_t      capacity;
   size_t      count;
} t_envp;

typedef struct s_minishell
{
   t_envp         *env;
   char           *line;
   t_token_stream *ts;
   t_ast          *ast;
   t_pipeline     pipeline;
   t_heredoc      heredoc;
   int            last_status;
} t_minishell;

void    minishell_free(t_minishell *sh);

#endif //MINISHELL_MINISHELL_H