//
// Created by wlucas-f on 11/16/25.
//

#include <stddef.h>
#include <unistd.h>
//#include <bits/signum-generic.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

extern volatile int sig;

void sigint_behaviour(void)
{
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void sigquit_behaviour(void)
{
    sig = 0;
    rl_replace_line("2", 0);
    rl_on_new_line();
}

void signal_handler(int sigg)
{
    printf("ENTREI AQUI CARALHO\n");
    if (sigg == SIGINT)
        sigint_behaviour();
    if (sigg == SIGABRT)
        sigquit_behaviour();
}

