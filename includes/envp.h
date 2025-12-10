/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:52:29 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:52:51 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVP_H
# define ENVP_H

# include <stdbool.h>
# include <stddef.h>

# define EXPORT 1

typedef struct s_var
{
	char			*name;
	char			*value;
	bool			export;
	size_t			len;
	struct s_var	*next;
	struct s_var	*prev;
}					t_var;

typedef struct s_envp
{
	t_var			*head;
	size_t			count;
}					t_envp;

// Core operations
t_var				*envp_getvar(t_envp *env, const char *name);
char				*envp_getvar_value(t_envp *env, const char *name,
						int last_status);
t_var				*envp_setvar(t_envp *env, const char *name,
						const char *value, int flags);
t_var				*envp_appendvar(t_envp *env, const char *name,
						const char *value, int flags);
bool				envp_unsetvar(t_envp *env, const char *name);

// API functions
t_var				*envp_setvar_str(t_envp *env, const char *assign,
						int last_status, int flags);
t_var				*envp_appendvar_str(t_envp *env, const char *assign,
						int last_status, int flags);

// Utils
char				**get_envp_array(t_envp *env, bool populated_only);
void				free_envp(t_envp *env);
size_t				key_len(const char *str);

// Internal helpers
t_var				*new_var(const char *name, char *value, bool export);
t_var				*envp_push(t_envp *env, t_var *node);

#endif
