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
int     exec_node(t_minishell* sh, const t_ast* node);
int     exec_command_line(t_minishell *sh, const t_ast *node);
int     exec_logical(t_minishell *sh, const t_ast *node);
int     exec_pipeline(t_minishell *sh, const t_ast_list *cores);
int     exec_core(t_minishell* sh, const t_ast* core, bool in_fork);
int     exec_command(t_minishell *sh, const t_ast *core);
int     exec_builtin(t_minishell *sh, char **argv, int argc);
int     exec_export(const t_minishell *sh, char **argv, int argc);
int     exec_unset(const t_minishell *sh, char **argv, int argc);
int     exec_exit(t_minishell *sh, char **argv, int argc);
int     exec_echo(char **argv, int argc);
int     exec_pwd(const t_minishell *sh, char **argv, int argc);
int     exec_env(const t_minishell *sh, char **argv, int argc);
int     exec_cd(const t_minishell *sh, char **argv, int argc);
int     exec_grouping(t_minishell *sh, const t_ast *node);
int     exec_redirs(t_minishell* sh, const t_ast_list* r);
int     get_redir_fd(t_token_type r);

// Commands execution helpers
char    *find_path(t_minishell *sh, char* cmd);
char	**argv_to_arr(const t_minishell *sh, const char **iter);
void	free_argv(char **argv);
int     execve_wrapper(t_minishell* sh, char** argv, int argc);
bool    is_builtin(const char *word);

// Pipeline helpers

# define READ_END 0
# define WRITE_END 1
bool    is_core_builtin(const t_ast *core);
int     pipeline_fork_error(t_minishell *sh, const t_ast_list *curr);
int     wait_pids(const t_pipeline *pipeline);

// Error
void    print_open_error(const char *filename);
void    print_dup2_error(void);


#endif /* MINISHELL_EXECUTOR_H */
