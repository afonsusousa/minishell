/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 16:13:34 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 16:17:03 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

/* Export error functions */
int		export_invalid_option(const char *arg);
int		export_invalid_identifier(const char *arg);
bool	is_valid_var_name(const char *str);

#endif /* BUILTINS_H */
