#ifndef MINISHELL_PARSER_H
#define MINISHELL_PARSER_H

#include <stddef.h>
#include "tokens.h"
#include "ast.h"

typedef struct s_parser
{
    t_token_stream	ts;
} t_parser;

// Recursive Descent Parser
t_ast       *parse(const t_token *tokens, size_t count);
t_ast		*parse_command_line(t_parser *p);
t_ast		*parse_list(t_parser *p);
t_ast		*parse_or_list(t_parser *p);
t_ast		*parse_and_list(t_parser *p);
t_ast		*parse_pipeline(t_parser *p);
t_ast       *parse_core(t_parser *p);
t_ast		*parse_command(t_parser *p);
const char  **parse_assignments(t_parser *p);
t_ast		*parse_redir(t_parser *p);
t_ast		*parse_grouping(t_parser *p);
t_ast_list	*parse_core_redirs(t_parser *p);

// Utils
int         is_redir_token_type(t_token_type t);
int         is_redir_ahead(const t_parser *p);

// Helpers
t_ast       *ast_make_binary_node(t_ast_type type, t_ast *left, t_ast *right);
t_ast       *ast_make_command_line_node(t_ast *list, int terminator);
t_ast       *ast_make_pipeline_node(t_ast *first_core);
t_ast       *ast_make_grouping_node(t_ast *list);
t_ast       *ast_make_redir_node(t_token_type type);

#endif // MINISHELL_PARSER_H
