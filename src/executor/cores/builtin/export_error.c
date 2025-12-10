/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:13:34 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:16:58 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../lib/libft/libft.h"
#include "builtins.h"
#include <unistd.h>

int	export_invalid_option(const char *arg)
{
	write(2, "minishell: export: -", 20);
	write(2, arg + 1, 1);
	write(2, ": invalid option\n", 17);
	write(2, "export: usage: export [name[=value] ...]\n", 41);
	return (2);
}

int	export_invalid_identifier(const char *arg)
{
	write(2, "minishell: export: `", 20);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 26);
	return (1);
}
