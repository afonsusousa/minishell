/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: executor <student@42>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 00:00:00 by executor          #+#    #+#             */
/*   Updated: 2025/10/07 00:00:00 by executor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>

#include "../includes/executor.h"
#include <string.h>
#include "../includes/ast.h"
#include "../includes/envp.h"
#include "../includes/subst.h"

static int	exec_node(t_minishell *sh, t_ast *node);
static int	exec_redirs(t_minishell *sh, t_ast_list *r);

static size_t  words_count(t_ast_list *w)
{
	size_t c;

	c = 0;
	while (w)
	{
		if (w->node && w->node->type == AST_WORD)
			c++;
		w = w->next;
	}
	return (c);
}

//TODO: expand wildcards here
char	**words_to_argv(t_ast_list *words)
{
	char		**argv;
	size_t		count;
	size_t		i;

	count = words_count(words);
	if (!count)
		return (NULL);
	argv = (char **)calloc(count + 1, sizeof(char *));
	if (!argv)
		return (NULL);
	i = 0;
	while (words)
	{
		if (words->node && words->node->type == AST_WORD)
			argv[i++] = (char *)words->node->as.leaf.text;
		words = words->next;
	}
	argv[i] = NULL;
	return (argv);
}

void	free_argv(char **argv)
{
	free(argv);
}

static char **envp_to_array(const t_envp *env, const t_envp *local_env)
{
	char	**arr;
	size_t	i;

	if (!env || !env->count)
		return (NULL);
	arr = (char **)calloc(env->count + local_env->count + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	i = 0;
	while (i < env->count)
	{
		arr[i] = env->vars[i].str;
		i++;
	}
	while (i < local_env->count)
	{
		arr[i] = local_env->vars[i - env->count].str;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

static int  execve_wrapper(char **argv, const t_envp *env, const t_envp *local_env)
{
	char	**env_arr;

	if (!argv || !argv[0])
		return (0);
	env_arr = envp_to_array(env, local_env);
	execve(argv[0], argv, env_arr);
	perror("execve");
	exit(127);
}

static int get_redir_fd(t_token_type r)
{
	if (r == TOK_REDIR_OUT || r == TOK_REDIR_APPEND)
		return (STDOUT_FILENO);
	return (STDIN_FILENO);
}
static int exec_redirs(t_minishell *sh, t_ast_list *r)
{
	int	ret;
	int	fd;
	char *filename;

	ret = 0;
	while (r)
	{
		filename = expanded_str(sh->env, r->node->as.redir.target->as.leaf.text);
		if (r->node->as.redir.kind == TOK_REDIR_OUT)
			fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (r->node->as.redir.kind == TOK_REDIR_IN)
			fd = open(filename, O_RDONLY);
		else if (r->node->as.redir.kind == TOK_REDIR_APPEND)
			fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = -1;
		if (dup2(fd, get_redir_fd(r->node->as.redir.kind)) < 0)
		{
			close(fd);
			free(filename);
			return (1);
		}
		if (fd < 0)
		{
			//error
			return (1);
		}
		close(fd);
		free(filename);
		r = r->next;
	}
	return (0);
}

static int exec_assignments(t_minishell *sh, t_ast_list *a, t_envp *env, bool global)
{
	char *text;

	if (global)
		env = sh->env;
	while (a)
	{
		text = envp_text_expand_wildcard(a->node->as.leaf.text);
		envp_elem_append(env, text);
		free(text);
		a = a->next;
	}
	return (0);
}

int	exec_simple_command(t_minishell *sh, t_ast *node)
{
	char	**argv;
	int		status;
	t_envp  local_env;

	memset(&local_env, 0, sizeof(t_envp));
	if (!node || node->type != AST_SIMPLE_COMMAND)
		return (1);
	argv = words_to_argv(node->as.simple_command.words);
	if (exec_redirs(sh, node->as.simple_command.redirs))
		return (1);
	if (exec_assignments(sh, node->as.simple_command.assignments,
							&local_env, argv == NULL))
		return (1);
	if (!argv)
		return (0);
	status = execve_wrapper(argv, sh->env, &local_env);
	free_argv(argv);
	sh->last_status = status;
	return (status);
}

//TODO: don't fork if running in pipeline!
int exec_grouping(t_minishell *sh, t_ast *node)
{
	int		status;
	pid_t	pid;

	pid = fork();
	exec_redirs(sh, node->as.command.redirs);
	status = exec_node(sh, node->as.grouping.list);
	return (status);
}

int	exec_command(t_minishell *sh, t_ast *node)
{
	int status;

	if (!node || node->type != AST_COMMAND)
		return (1);
	status = 0;
	if (node->as.command.core->type == AST_SIMPLE_COMMAND)
		status = exec_simple_command(sh, node->as.command.core);
	else if (node->as.command.core->type == AST_GROUPING)
		status = exec_grouping(sh, node);
	sh->last_status = status;
	return (status);
}

static inline int wait_pids(const pid_t *pids, const size_t count)
{
	size_t	i;
	int		status;
	int		st;

	i = 0;
	status = 0;
	while (i < count)
	{
		if (waitpid(pids[i], &st, 0) > 0 && i == count - 1)
		{
			if (WIFEXITED(st))
				status = WEXITSTATUS(st);
			else if (WIFSIGNALED(st))
				status = 128 + WTERMSIG(st);
		}
		i++;
	}
	return (status);
}

//TODO: SINGLE COMMAND NO REDIRS
int exec_pipeline(t_minishell *sh, const t_ast_list *cmds)
{
    int         prev_read = -1;
    int         pipefd[2];
    pid_t       pids[256];
    int         count = 0;

    while (cmds) {
        if (cmds->next && pipe(pipefd) < 0)
        {
            perror("pipe");
            if (prev_read != -1)
            	close(prev_read);
            return (1);
        }
    	pids[count] = fork();
        if (pids[count] < 0) {
            perror("fork");
            if (cmds->next)
            {
	            close(pipefd[0]);
            	close(pipefd[1]);
            }
            if (prev_read != -1)
            	close(prev_read);
            return (1);
        }
        if (pids[count] == 0)
        {
            if (prev_read != -1)
            {
                if (dup2(prev_read, STDIN_FILENO) < 0)
                    exit(1);
            }
            if (cmds->next)
            {
                close(pipefd[0]);
                if (dup2(pipefd[1], STDOUT_FILENO) < 0)
                    exit(1);
            }
            if (prev_read != -1)
            	close(prev_read);
            if (!cmds->next)
            	close(pipefd[1]);
        	exec_command(sh, cmds->node);
        }
        if (prev_read != -1)
            close(prev_read);
        if (!cmds->next)
        {
            close(pipefd[1]);
            prev_read = pipefd[0];
        }
    	else
            prev_read = -1;
    	count++;
        cmds = cmds->next;
    }
    if (prev_read != -1)
        close(prev_read);
    sh->last_status = wait_pids(pids, count);
	return (sh->last_status);
}

int	exec_binop(t_minishell *sh, t_ast *node)
{
	int left_status;

	if (!node || (node->type != AST_AND_LIST && node->type != AST_OR_LIST))
		return (1);
	left_status = exec_node(sh, node->as.binop.left);
	if (node->type == AST_AND_LIST)
	{
		if (left_status == 0)
			return (exec_node(sh, node->as.binop.right));
		return (left_status);
	}
	if (node->type == AST_OR_LIST)
	{
		if (left_status != 0)
			return (exec_node(sh, node->as.binop.right));
		return (left_status);
	}
	return (1);
}

int	exec_command_line(t_minishell *sh, t_ast *node)
{
	if (!node || node->type != AST_COMMAND_LINE)
		return (1);
	return (exec_node(sh, node->as.command_line.list));
}

static int exec_node(t_minishell *sh, t_ast *node)
{
	if (!node)
		return (1);
	if (node->type == AST_COMMAND_LINE)
		return (exec_command_line(sh, node));
	if (node->type == AST_PIPELINE)
		return (exec_pipeline(sh, node));
	if (node->type == AST_COMMAND)
		return (exec_command(sh, node));
	if (node->type == AST_SIMPLE_COMMAND)
		return (exec_simple_command(sh, node));
	if (node->type == AST_GROUPING)
		return (exec_grouping(sh, node));
	if (node->type == AST_AND_LIST || node->type == AST_OR_LIST)
		return (exec_binop(sh, node));
	return (0);
}

int	exec_ast(t_minishell *sh, t_ast *root)
{
	int status;

	status = exec_node(sh, root);
	sh->last_status = status;
	return (status);
}
