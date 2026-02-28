/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:32:41 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 19:03:01 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/executor.h"
#include "../../../../includes/utils.h"
#include <stdbool.h>

bool	is_builtin(const char *word)
{
	if (!word)
		return (false);
	return (ft_strcmp("export", word) == 0 || ft_strcmp("unset", word) == 0
		|| ft_strcmp("cd", word) == 0 || ft_strcmp("echo", word) == 0
		|| ft_strcmp("env", word) == 0 || ft_strcmp("pwd", word) == 0
		|| ft_strcmp("exit", word) == 0 || ft_strcmp("..", word) == 0
		|| ft_strcmp(".", word) == 0);
}
