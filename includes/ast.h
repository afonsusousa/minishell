#ifndef MINISHELL_AST_H
#define MINISHELL_AST_H

#include <stddef.h>

// Forward declarations
struct s_ast;
struct s_redir;
struct s_word;

// Node type enumeration representing high-level grammar constructs.
typedef enum e_ast_type {
    AST_COMMAND_LINE,
    AST_LIST,
    AST_OR_LIST,
    AST_AND_LIST,
    AST_PIPELINE,
    AST_COMMAND,        // command_core + trailing redirs already merged
    AST_SIMPLE_COMMAND,
    AST_GROUPING,
    AST_ASSIGNMENT,     // Single assignment (NAME=VALUE) token
    AST_WORD,           // Word token
    AST_REDIR,          // Redirection node (<, >, >>)
    AST_HEREDOC         // Heredoc delimiter (<<)
} t_ast_type;

typedef enum e_redir_kind {
    R_IN,
    R_OUT,
    R_APPEND,
    R_HEREDOC_KIND   // << (separate from AST_HEREDOC node type used if you prefer)
} t_redir_kind;

typedef struct s_ast_list {
    struct s_ast       *node;
    struct s_ast_list  *next;
} t_ast_list;

typedef struct s_redir {
    t_redir_kind kind;
    struct s_ast *target;
} t_redir;

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
            size_t      len;
        } token_like;

        struct {                // AST_REDIR / AST_HEREDOC (normalized redirection)
            t_redir_kind kind;
            struct s_ast *target; // AST_WORD
        } redir;

        struct {                // AST_OR_LIST / AST_AND_LIST binary nodes
            struct s_ast *left;
            struct s_ast *right;
        } binop;
    } as;
} t_ast;

t_ast *ast_new(t_ast_type type);
void   ast_list_push(t_ast_list **head, t_ast *node);
void   ast_list_free(t_ast_list *lst);

void   ast_free(t_ast *node);

#endif // MINISHELL_AST_H
