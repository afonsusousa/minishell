//
// Created by afonsusousa on 11/17/25.
//

#ifndef MINISHELL_SIG_H
#define MINISHELL_SIG_H

#include <signal.h>


void signal_setup(void);
void sigint_handler(int signum);
void sigint_heredoc_handler(int signum);

int signal_set_heredoc_mode(struct sigaction *saved_sa);
int signal_restore(int signum, struct sigaction *saved_sa);

#endif // MINISHELL_SIG_H
