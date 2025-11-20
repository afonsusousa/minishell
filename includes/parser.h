#ifndef MINISHELL_PARSER_H
#define MINISHELL_PARSER_H

#include <stddef.h>
#include "tokens.h"
#include "ast.h"

// Recursive Descent Parser
void        parse(t_minishell *sh);
t_ast		*parse_command_line(t_minishell *sh);
t_ast		*parse_list(t_minishell *s);
t_ast		*parse_or_list(t_minishell *sh);
t_ast		*parse_and_list(t_minishell *sh);
t_ast		*parse_pipeline(t_minishell *sh);
t_ast       *parse_core(t_minishell *sh);
t_ast		*parse_command(t_minishell *sh);
const char  **parse_assignments(t_minishell *sh);
t_ast		*parse_redir(t_minishell *sh);
t_ast		*parse_grouping(t_minishell *sh);
t_ast_list	*parse_core_redirs(t_minishell *sh);

// Utils
t_token_type is_redir_token_type(t_token_stream *ts);
int          is_redir_ahead(const t_token_stream *ts);
void        heredoc_setup(t_minishell *sh, int heredoc[2]);

// Helpers
t_ast       *ast_make_binary_node(t_ast_type type, t_ast *left, t_ast *right);
t_ast       *ast_make_command_line_node(t_ast *list, int terminator);
t_ast       *ast_make_pipeline_node(t_ast *first_core);
t_ast       *ast_make_grouping_node(t_ast *list);
t_ast       *ast_make_redir_node(t_token_type type);
t_ast       *ast_make_command_node(const char **assignments, char **argv, int argc, t_ast_list *redirs);

// Error
void        parser_abort(t_minishell *sh);
void        parser_abort_error(t_minishell *sh);

#endif // MINISHELL_PARSER_H
