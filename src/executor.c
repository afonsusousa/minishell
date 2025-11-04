#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include "../includes/executor.h"
#include "../lib/libft/libft.h"
#include <string.h>
#include "../includes/ast.h"
#include "../includes/envp.h"
#include "../includes/subst.h"
#include "../includes/utils.h"

static int exec_node(t_minishell* sh, t_ast* node);
static int exec_redirs(t_minishell* sh, t_ast_list* r, bool duplicate);

static size_t words_count(t_minishell* sh, const char **w)
{
    size_t c;
    (void) sh;

    c = 0;
    while (w && *w++)
        c++;
    return (c);
}

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1++ && *s2++)
        if (*s1 != *s2)
            return (*s1 - *s2);
    return (*s1 - *s2);
}

bool is_builtin (char *word)
{
    if (!word)
        return (false);
    return (ft_strcmp("export", word) == 0
        || ft_strcmp("unset", word) == 0
        || ft_strcmp("cd", word) == 0
        || ft_strcmp("echo", word) == 0
        || ft_strcmp("env", word) == 0
        || ft_strcmp("pwd", word) == 0
        || ft_strcmp("exit", word) == 0
        );
}

//TODO: expand wildcards here
char** words_to_argv(t_minishell* sh, const  char **words)
{
    char** argv;
    size_t count;
    size_t i;

    count = words_count(sh, words);
    if (!count)
        return (NULL);
    argv = (char**)calloc(count + 1, sizeof(char*));
    if (!argv)
        return (NULL);
    i = 0;
    while (words && *words)
        argv[i++] = ft_strdup(*words++);
    argv[i] = NULL;
    return (argv);
}

void free_argv(char** argv)
{
    free(argv);
}

//TODO: local access (./)
char* find_path(char* cmd, char** envp)
{
    size_t i;
    char** split_path;
    char* try;

    i = 0;
    while (*envp && (ft_strncmp("PATH=", *envp, 5)))
        envp++;
    if (!*envp)
        return (NULL);
    split_path = ft_split(*envp + 5, ':');
    if (!split_path)
        return (NULL);
    while (split_path[i])
    {
        try = strjoin_three(split_path[i++], "/", cmd);
        if (!try)
            return (free_until_null(&split_path), NULL);
        if (access(try, X_OK) == 0)
            return (free_until_null(&split_path), try);
        free(try);
    }
    return (free_until_null(&split_path), ft_strdup(cmd));
}

int export(t_minishell *sh, char **argv)
{
    (void) sh, (void) argv;
    return (1);
}
int exec_builtin(t_minishell *sh, char **argv)
{
    if (ft_strcmp("export", argv[0]) == 0)
        return (export(sh, argv));
    return (1);
}

static int execve_wrapper(t_minishell* sh, char** argv)
{
    char** env_arr;

    if (!argv || !argv[0])
        return (0);
    env_arr = get_envp_array(sh->env);
    env_arr = strjoinjoin(env_arr, get_envp_array(sh->ctx));
    if (is_builtin(argv[0]))
        return (exec_builtin(sh, argv));
    argv[0] = find_path(argv[0], env_arr);
    minishell_free(sh);
    execve(argv[0], argv, env_arr);
    perror("execve");
    exit(127);
}

static int get_redir_fd(t_token_type r)
{
    if (r == TOK_REDIR_OUT || r == TOK_REDIR_APPEND)
        return (STDOUT_FILENO);
    return (STDIN_FILENO);
}

static int exec_redirs(t_minishell* sh, t_ast_list* r, bool duplicate)
{
    int fd;
    char* filename;
    memset(&sh->heredoc, 0, sizeof(t_heredoc));
    while (r)
    {
        filename = (char *) r->node->as.redir.target;
        if (r->node->as.redir.kind == TOK_REDIR_OUT)
            fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (r->node->as.redir.kind == TOK_REDIR_IN)
            fd = open(filename, O_RDONLY);
        else if (r->node->as.redir.kind == TOK_REDIR_APPEND)
            fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else if (r->node->as.redir.kind == TOK_HEREDOC)
            fd = heredoc_fd(sh, r->node->as.redir.target);
        else
            fd = -1;
        if (duplicate && dup2(fd, get_redir_fd(r->node->as.redir.kind)) < 0)
        {
            close(fd);
            return (1);
        }
        if (fd < 0)
        {
            //error
            return (1);
        }
        close(fd);
        r = r->next;
    }
    return (0);
}

static int exec_assignments(t_minishell* sh, const char **a, bool context)
{
    t_envp *env;

    env = sh->env;
    if (context)
        env = sh->ctx;
    while (a && *a)
        if (envp_setvar(env, *a++, context) == NULL)
                return (1);;
    return (0);
}

int exec_command(t_minishell* sh, t_ast* node, bool in_fork)
{
    char** argv;
    int status;
    t_envp local_env;
    (void)in_fork;

    memset(sh->ctx, 0, sizeof(t_envp));
    if (!node || node->type != AST_COMMAND)
        return (1);
    argv = words_to_argv(sh, node->as.command.words);
    if (argv && is_builtin(argv[0]))
        return (exec_builtin(sh, argv));
    if (exec_redirs(sh, node->as.command.redirs, in_fork))
        return (1);
    if (exec_assignments(sh, node->as.command.assignments, argv != NULL))
        return (1);
    if (!argv)
        return (0);
    status = execve_wrapper(sh, argv);
    //restore_fds();
    free_argv(argv);
    free_envp(&local_env);
    sh->last_status = status;
    return (status);
}

int exec_grouping(t_minishell* sh, t_ast* node, bool in_fork)
{
    int status;
    exec_redirs(sh, node->as.command.redirs, in_fork);
    status = exec_node(sh, node->as.grouping.list);
    return (status);
}

int exec_core(t_minishell* sh, t_ast* core, bool in_fork)
{
    int status;

    if (!core || (core->type != AST_COMMAND && core->type != AST_GROUPING))
        return (1);
    status = 0;
    if (core->type == AST_COMMAND)
        status = exec_command(sh, core, in_fork);
    else
        status = exec_grouping(sh, core, in_fork);
    sh->last_status = status;
    return (status);
}

static inline int wait_pids(const t_pipeline *pipeline)
{
    size_t i;
    int status;
    int st;

    i = 0;
    status = 0;
    while (i < pipeline->count)
    {
        if (waitpid(pipeline->pids[i], &st, 0) > 0 && i == pipeline->count - 1)
        {
            if (WIFEXITED(st))
                status = WEXITSTATUS(st);
            else if (WIFSIGNALED(st))
                status = 128 + WTERMSIG(st);
        }
        i++;
    }
    return (status);
}
size_t ft_max(size_t a, size_t b)
{
   if (a > b)
       return a;
    return b;
}

//TODO: buitlins will not fork
int exec_pipeline(t_minishell* sh, const t_ast_list* cores)
{
    int status;
    t_pipeline *pipeline;

    pipeline = &sh->pipeline;
    memset(pipeline, 0, sizeof(t_pipeline));
    pipeline->prev_read = -1;
    // expand arguments
    // exec assignments if no cmd
    //
    //
    if (!cores->next)
    {
        if (cores->node->type == AST_COMMAND
            && (!cores->node->as.command.words
                || is_builtin((char *)cores->node->as.command.words[0])))
            return (exec_command(sh, cores->node, false));
    }
    while (cores)
    {
        if (cores->next && pipe(pipeline->pipefd) < 0)
        {
            perror("pipe");
            if (pipeline->prev_read != -1)
                close(pipeline->prev_read);
            return (1);
        }
        pipeline->pids[pipeline->count] = fork();
        if (pipeline->pids[pipeline->count] < 0)
        {
            perror("fork");
            if (cores->next)
            {
                close(pipeline->pipefd[0]);
                close(pipeline->pipefd[1]);
            }
            if (pipeline->prev_read != -1)
                close(pipeline->prev_read);
            return (1);
        }
        if (pipeline->pids[pipeline->count] == 0)
        {
            if (pipeline->prev_read != -1)
            {
                if (dup2(pipeline->prev_read, STDIN_FILENO) < 0)
                    exit(1);
                close(pipeline->prev_read);
            }
            if (cores->next)
            {
                close(pipeline->pipefd[0]);
                if (dup2(pipeline->pipefd[1], STDOUT_FILENO) < 0)
                    exit(1);
                close(pipeline->pipefd[1]);
            }
            status = exec_core(sh, cores->node, true);
            minishell_free(sh);
            exit(status);
        }
        if (pipeline->prev_read != -1)
            close(pipeline->prev_read);
        if (cores->next)
        {
            close(pipeline->pipefd[1]);
            pipeline->prev_read = pipeline->pipefd[0];
        }
        pipeline->count++;
        cores = cores->next;
    }
    if (pipeline->prev_read != -1)
        close(pipeline->prev_read);
    status = wait_pids(pipeline);
    sh->last_status = status;
    return (status);
}

int exec_binop(t_minishell* sh, t_ast* node)
{
    int left_status;

    if (!node || (node->type != AST_AND_LIST && node->type != AST_OR_LIST))
        return (1);
    left_status = exec_node(sh, node->as.binop.left);
    if (node->type == AST_AND_LIST)
    {
        if (left_status == 0)
            return (exec_node(sh, node->as.binop.right));
        return (left_status);
    }
    if (node->type == AST_OR_LIST)
    {
        if (left_status != 0)
            return (exec_node(sh, node->as.binop.right));
        return (left_status);
    }
    return (1);
}

int exec_command_line(t_minishell* sh, t_ast* node)
{
    if (!node || node->type != AST_COMMAND_LINE)
        return (1);
    return (exec_node(sh, node->as.command_line.list));
}

static int exec_node(t_minishell* sh, t_ast* node)
{
    if (!node)
        return (1);
    if (node->type == AST_COMMAND_LINE)
        return (exec_command_line(sh, node));
    if (node->type == AST_PIPELINE)
        return (exec_pipeline(sh, node->as.pipeline.cores));
    if (node->type == AST_COMMAND)
        return (exec_command(sh, node, false));
    if (node->type == AST_GROUPING)
        return (exec_grouping(sh, node, false));
    if (node->type == AST_AND_LIST || node->type == AST_OR_LIST)
        return (exec_binop(sh, node));
    return (0);
}

int exec_ast(t_minishell* sh)
{
    int status;

    status = exec_node(sh, sh->ast);
    sh->last_status = status;
    return (status);
}
