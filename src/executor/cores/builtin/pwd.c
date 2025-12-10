#include "../../../../includes/executor.h"
#include "../../../../includes/minishell.h"
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>

int	exec_pwd(const t_minishell *sh, char **argv, const int argc)
{
	char	pwd[ARG_MAX];

	(void)sh;
	(void)argv;
	(void)argc;
	getcwd(pwd, ARG_MAX);
	printf("%s\n", pwd);
	return (0);
}
