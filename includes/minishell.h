//
// Created by afonsusousa on 9/27/25.
//

#ifndef MINISHELL_MINISHELL_H
#define MINISHELL_MINISHELL_H
#include <stddef.h>

typedef struct s_envp_elem
{
   char  *tag;
   char  *value;
   bool  export;
} t_envp_elem;

typedef struct s_envp
{
   t_envp_elem *data;
   size_t      capacity;
   size_t      count;
} t_envp;

typedef struct s_minishell
{
   t_envp env;
} t_minishell;
#endif //MINISHELL_MINISHELL_H