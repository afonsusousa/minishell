#ifndef MINISHELL_AST_H
#define MINISHELL_AST_H

#include <stddef.h>
#include "lexer.h"

struct s_ast;
struct s_redir;
struct s_word;

typedef enum e_ast_type {
    AST_COMMAND_LINE,
    AST_LIST,
    AST_OR_LIST,
    AST_AND_LIST,
    AST_PIPELINE,
    AST_COMMAND,
    AST_SIMPLE_COMMAND,
    AST_GROUPING,
    AST_ASSIGNMENT,
    AST_WORD,
    AST_REDIR,
    AST_HEREDOC
} t_ast_type;

typedef struct s_ast_list {
    struct s_ast       *node;
    struct s_ast_list  *next;
} t_ast_list;

typedef struct s_ast {
    t_ast_type type;
    union {

        struct {                // AST_COMMAND_LINE
            struct s_ast *list; // child list
            int terminator;     // ';' '&' or 0 for none
        } command_line;

        struct {                // AST_PIPELINE
            t_ast_list *commands; // sequence of command nodes
        } pipeline;

        struct {                // AST_COMMAND
            struct s_ast *core; // grouping or simple_command
            t_ast_list   *redirs; // trailing redirections
        } command;

        struct {                // AST_SIMPLE_COMMAND
            t_ast_list *assignments; // leading assignment nodes (AST_ASSIGNMENT)
            t_ast_list *words;       // command name + args (AST_WORD)
            t_ast_list *redirs;      // interleaved redirs (AST_REDIR / AST_HEREDOC)
        } simple_command;

        struct {                // AST_GROUPING
            struct s_ast *list; // nested list
        } grouping;

        struct {                // AST_ASSIGNMENT / AST_WORD / HEREDOC delimiter or filename
            const char *text;
            bool        quoted;
        } leaf;

        struct {                // AST_REDIR / AST_HEREDOC (normalized redirection)
            t_token_type kind;
            struct s_ast *target; // AST_WORD
        } redir;

        struct {                // AST_OR_LIST / AST_AND_LIST binary nodes
            struct s_ast *left;
            struct s_ast *right;
        } binop;
    } as;
} t_ast;

t_ast *ast_new(t_ast_type type);
//static t_ast_list	*ast_list_new(t_ast *n);
t_ast_list *ast_list_push(t_ast_list **head, t_ast *node);
void   ast_list_free(t_ast_list *lst);

void   ast_free(t_ast *node);

#endif // MINISHELL_AST_H
