/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell blueprint <student@42>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 00:00:00 by executor          #+#    #+#             */
/*   Updated: 2025/10/07 00:00:00 by executor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_EXECUTOR_H
# define MINISHELL_EXECUTOR_H

# include "ast.h"
# include "minishell.h"

int     exec_ast(t_minishell *sh);
int     exec_node(t_minishell* sh, t_ast* node);
int     exec_command_line(t_minishell *sh, t_ast *node);
int     exec_logical(t_minishell *sh, t_ast *node);
int     exec_pipeline(t_minishell *sh, const t_ast_list *cores);
int     exec_core(t_minishell* sh, t_ast* core, bool in_fork);
int     exec_command(t_minishell *sh, t_ast *core, bool in_fork);
int     exec_builtin(t_minishell *sh, char **argv, int argc);
int     exec_grouping(t_minishell *sh, t_ast *node, bool in_fork);
int     exec_redirs(t_minishell* sh, t_ast_list* r, bool duplicate);
int     get_redir_fd(t_token_type r);

// Commands execution helpers
char    *find_path(char* cmd, char** envp);
char	**argv_to_arr(t_minishell *sh, const char **words);
void	free_argv(char **argv);
int     execve_wrapper(t_minishell* sh, char** argv, int argc);
bool    is_builtin(char *word);

#endif /* MINISHELL_EXECUTOR_H */
