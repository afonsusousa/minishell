#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>

#include "../includes/executor.h"
#include <string.h>
#include "../includes/ast.h"
#include "../includes/envp.h"
#include "../includes/subst.h"
#include "../includes/utils.h"

static int exec_node(t_minishell* sh, t_ast* node);
static int exec_redirs(t_minishell* sh, t_ast_list* r, bool duplicate);
static size_t words_count(t_minishell* sh, t_ast_list* w)
{
    size_t c;
    (void) sh;

    c = 0;
    while (w)
    {
        if (w->node && w->node->type == AST_WORD)
            c += count_words(w->node->as.leaf.text, ' ');
        w = w->next;
    }
    return (c);
}

//TODO: expand wildcards here
char** words_to_argv(t_minishell* sh, t_ast_list* words)
{
    char** argv;
    char** split;
    size_t count;
    size_t i;

    count = words_count(sh, words);
    if (!count)
        return (NULL);
    argv = (char**)calloc(count + 1, sizeof(char*));
    if (!argv)
        return (NULL);
    i = 0;
    while (words)
    {
        if (words->node && words->node->type == AST_WORD)
        {
            split = ft_split(words->node->as.leaf.text, ' ');
            while (split && *split)
                argv[i++] = *split++;
        }
        words = words->next;
    }
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

static int execve_wrapper(t_minishell* sh, char** argv)
{
    char** env_arr;

    if (!argv || !argv[0])
        return (0);
    env_arr = get_envp_array(sh->env);
    env_arr = strjoinjoin(env_arr, get_envp_array(sh->ctx));
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
        filename = (char *) r->node->as.redir.target->as.leaf.text;
        if (r->node->as.redir.kind == TOK_REDIR_OUT)
            fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (r->node->as.redir.kind == TOK_REDIR_IN)
            fd = open(filename, O_RDONLY);
        else if (r->node->as.redir.kind == TOK_REDIR_APPEND)
            fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else if (r->node->as.redir.kind == TOK_HEREDOC)
            fd = heredoc_fd(sh, r->node->as.redir.target->as.leaf.text);
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

static int exec_assignments(t_minishell* sh, t_ast_list* a, bool global)
{
    t_envp *env;

    env = sh->ctx;
    if (global)
        env = sh->env;
    while (a)
    {
        if (envp_append_var(env, a->node->as.leaf.text, !global) == NULL)
            return (1);
        a = a->next;
    }
    return (0);
}

int exec_simple_command(t_minishell* sh, t_ast* node, bool in_fork)
{
    char** argv;
    int status;
    t_envp local_env;
    (void)in_fork;

    memset(&sh->ctx, 0, sizeof(t_envp));
    if (!node || node->type != AST_SIMPLE_COMMAND)
        return (1);
    argv = words_to_argv(sh, node->as.simple_command.words);
    if (exec_redirs(sh, node->as.simple_command.redirs, in_fork))
        return (1);
    if (exec_assignments(sh, node->as.simple_command.assignments, argv == NULL))
        return (1);
    if (!argv)
        return (0);
    status = execve_wrapper(sh, argv);
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

int exec_command(t_minishell* sh, t_ast* node, bool in_fork)
{
    int status;

    if (!node || node->type != AST_COMMAND)
        return (1);
    status = 0;
    if (node->as.command.core->type == AST_SIMPLE_COMMAND)
        status = exec_simple_command(sh, node->as.command.core, in_fork);
    else if (node->as.command.core->type == AST_GROUPING)
        status = exec_grouping(sh, node, in_fork);
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

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1++ && *s2++)
        if (*s1 != *s2)
            return (*s1 - *s2);
    return (*s1 - *s2);
}

bool is_builtin (t_ast *cmd)
{
    const char *word;

    if (!cmd || !cmd->as.simple_command.words)
        return (false);
    word = cmd->as.simple_command.words->node->as.leaf.text;
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

//TODO: buitlins will not fork
int exec_pipeline(t_minishell* sh, const t_ast_list* cmds)
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
    if (!cmds->next)
    {
        if (cmds->node->as.command.core->type == AST_SIMPLE_COMMAND
            && (!cmds->node->as.command.core->as.simple_command.words || is_builtin(cmds->node->as.command.core)))
            return (exec_simple_command(sh, cmds->node->as.command.core, false));
    }
    while (cmds)
    {
        if (cmds->next && pipe(pipeline->pipefd) < 0)
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
            if (cmds->next)
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
            if (cmds->next)
            {
                close(pipeline->pipefd[0]);
                if (dup2(pipeline->pipefd[1], STDOUT_FILENO) < 0)
                    exit(1);
                close(pipeline->pipefd[1]);
            }
            status = exec_command(sh, cmds->node, true);
            minishell_free(sh);
            exit(status);
        }
        if (pipeline->prev_read != -1)
            close(pipeline->prev_read);
        if (cmds->next)
        {
            close(pipeline->pipefd[1]);
            pipeline->prev_read = pipeline->pipefd[0];
        }
        pipeline->count++;
        cmds = cmds->next;
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
        return (exec_pipeline(sh, node->as.pipeline.commands));
    if (node->type == AST_COMMAND)
        return (exec_command(sh, node, false));
    if (node->type == AST_SIMPLE_COMMAND)
        return (exec_simple_command(sh, node, false));
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
