/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:33:15 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:33:17 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/executor.h"
#include "../../../includes/globbing.h"
#include "../../../includes/minishell.h"
#include "../../../includes/utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int	open_redir_file(const t_token_type kind, const char *filename)
{
	int	fd;

	fd = -1;
	if (ft_strcmp(filename, "&0") == 0)
		fd = STDIN_FILENO;
	else if (ft_strcmp(filename, "&1") == 0)
		fd = STDOUT_FILENO;
	else if (ft_strcmp(filename, "&2") == 0)
		fd = STDERR_FILENO;
	else if (kind == TOK_REDIR_APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (kind == TOK_REDIR_OUT || kind == TOK_REDIR_0_OUT
		|| kind == TOK_REDIR_1_OUT || kind == TOK_REDIR_2_OUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (kind == TOK_REDIR_IN || kind == TOK_REDIR_0_IN
		|| kind == TOK_REDIR_1_IN || kind == TOK_REDIR_2_IN)
		fd = open(filename, O_RDONLY);
	return (fd);
}

static int	handle_heredoc_redir(const t_ast *node)
{
	int	fd;

	fd = node->u_as.s_redir.u_target.heredoc[0];
	if (fd < 0)
		return (0);
	if (dup2(fd, get_redir_fd(node->u_as.s_redir.kind)) < 0)
	{
		print_dup2_error();
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static int	handle_file_redir(t_minishell *sh, const t_ast *node)
{
	int		fd;
	t_word	**filename_words;
	char	*filename;

	filename_words = expand_argv_word(sh,
			(char *)node->u_as.s_redir.u_target.file_name);
	if (filename_words && *filename_words && filename_words[1] != NULL)
		return (handle_ambiguous_redirect(node->u_as.s_redir.u_target.file_name,
				filename_words));
	if (!filename_words)
		return (0);
	filename = word_to_cstr(*filename_words);
	fd = open_redir_file(node->u_as.s_redir.kind, filename);
	if (fd < 0)
		return (handle_open_error(filename, filename_words));
	if (dup2(fd, get_redir_fd(node->u_as.s_redir.kind)) < 0)
		return (handle_dup2_error(fd, filename, filename_words));
	if (fd > 2)
		close(fd);
	free(filename);
	word_free_until_null(filename_words);
	return (0);
}

int	exec_redirs(t_minishell *sh, const t_ast_list *r)
{
	int result;

	memset(&sh->heredoc, 0, sizeof(t_heredoc));
	while (r)
	{
		if (r->node->u_as.s_redir.kind == TOK_HEREDOC)
			result = handle_heredoc_redir(r->node);
		else
			result = handle_file_redir(sh, r->node);
		if (result != 0)
			return (1);
		r = r->next;
	}
	return (0);
}