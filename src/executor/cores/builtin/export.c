/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:32:01 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:16:56 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/envp.h"
#include "../../../../includes/executor.h"
#include "../../../../includes/minishell.h"
#include "../../../../includes/utils.h"
#include "../../../../lib/libft/libft.h"
#include "builtins.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

int	print_exported_variables(const t_minishell *sh)
{
	char	**envp;
	int		size;

	size = 0;
	envp = get_envp_array(sh->env, false);
	while (envp && envp[size])
		size++;
	merge_sort((void *)envp, 0, size - 1, ft_memucmp);
	while (envp && *envp)
	{
		if (ft_strchr(*envp, '='))
			printf("declare -x %.*s=\"%s\"\n", (int)key_len(*envp), *envp,
				ft_strchr(*envp, '=') + 1);
		else
			printf("declare -x %s\n", *envp);
		envp++;
	}
	return (0);
}

bool	is_valid_var_name(const char *str)
{
	size_t	i;

	if (!str || !*str)
		return (false);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (false);
	i = 0;
	while (str[i] && str[i] != '=' && str[i] != '+')
	{
		if (!is_valid(str[i]))
			return (false);
		i++;
	}
	if (str[i] == '+' && str[i + 1] != '=')
		return (false);
	return (true);
}

bool	is_append(const char *str)
{
	size_t	i;

	i = 0;
	while (str && str[i++])
	{
		if (str[i] == '+' && str[i + 1] == '=' && is_valid(str[i - 1]))
			return (true);
		if (!is_valid(str[i]) && str[i] != '+')
			break ;
	}
	return (false);
}

int	exec_export(const t_minishell *sh, char **argv, const int argc)
{
	t_var	*var;
	int		status;

	(void)argc;
	if (argv[1] == NULL)
		return (print_exported_variables(sh));
	status = 0;
	while (*++argv != NULL)
	{
		if (**argv == '-')
			return (export_invalid_option(*argv));
		if (!is_valid_var_name(*argv))
		{
			status = export_invalid_identifier(*argv);
			continue ;
		}
		if (is_append(*argv))
			var = envp_appendvar_str(sh->env, *argv, sh->last_status, EXPORT);
		else
			var = envp_setvar_str(sh->env, *argv, sh->last_status, EXPORT);
		if (!var)
			status = 127;
	}
	return (status);
}
