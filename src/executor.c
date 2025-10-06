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

#include "../includes/executor.h"
#include "../includes/ast.h"
#include "../includes/envp.h"

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

static char **envp_to_array(const t_envp *env)
{
	char	**arr;
	size_t	i;

	if (!env || !env->count)
		return (NULL);
	arr = (char **)calloc(env->count + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	i = 0;
	while (i < env->count)
	{
		arr[i] = env->vars[i].str;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

static int  run_external(char **argv, const t_envp *env)
{
	pid_t	pid;
	int		status;
	char	**env_arr;

	if (!argv || !argv[0])
		return (0);
	pid = fork();
	if (pid < 0)
		return (errno);
	if (pid == 0)
	{
		env_arr = envp_to_array(env);
		execve(argv[0], argv, env_arr);
		perror("execve");
		exit(127);
	}
	if (waitpid(pid, &status, 0) < 0)
		return (errno);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static int exec_redirs(t_minishell *sh, t_ast_list *r)
{
	int ret;

	(void)sh;
	ret = 0;
	while (r)
	{
		//TODO: exec redirs, check now if redir target is wildcard
		if (ret)
			return (ret);
		r = r->next;
	}
	return (0);
}

static int exec_assignments(t_minishell *sh, t_ast_list *a)
{
	(void) sh;
	while (a)
	{
		//TODO: handle assignments
		a = a->next;
	}
	return (0);
}

int	exec_simple_command(t_minishell *sh, t_ast *node)
{
	char	**argv;
	int		status;

	if (!node || node->type != AST_SIMPLE_COMMAND)
		return (1);
	if (exec_assignments(sh, node->as.simple_command.assignments))
		return (1);
	if (exec_redirs(sh, node->as.simple_command.redirs))
		return (1);
	argv = words_to_argv(node->as.simple_command.words);
	if (!argv)
		return (0);
	status = run_external(argv, sh->env);
	free_argv(argv);
	sh->last_status = status;
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
		status = exec_node(sh, node->as.grouping.list);
	else
		status = exec_node(sh, node->as.command.core);
	//TODO: trailing redirs in case of grouping
	sh->last_status = status;
	return (status);
}

int	exec_pipeline(t_minishell *sh, t_ast *node)
{
	//TODO: figure this out from pipex
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
