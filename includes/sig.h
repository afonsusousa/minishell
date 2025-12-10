/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:39:57 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:40:33 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_SIG_H
# define MINISHELL_SIG_H

# include <signal.h>

void	signal_setup(void);
void	sigint_handler(int signum);
void	sigint_heredoc_handler(int signum);

#endif // MINISHELL_SIG_H
