#ifndef MINISHELL_PARSER_H
#define MINISHELL_PARSER_H

#include <stddef.h>
#include "tokens.h"
#include "ast.h"

typedef struct s_parser
{
    t_token_stream	ts;
} t_parser;

t_ast		*parse_command_line(t_parser *p);
t_ast		*parse_list(t_parser *p);
t_ast		*parse_or_list(t_parser *p);
t_ast		*parse_and_list(t_parser *p);
t_ast		*parse_pipeline(t_parser *p);
t_ast		*parse_redir(t_parser *p);
t_ast_list	*parse_command_redirs(t_parser *p);
t_ast		*parse_simple_command(t_parser *p);
t_ast		*parse_command(t_parser *p);
t_ast		*parse_grouping(t_parser *p);
t_ast_list	*parse_assignments(t_parser *p);
// Public parser entrypoint (may be implemented later)
t_ast *parse(const t_token *tokens, size_t count);

#endif // MINISHELL_PARSER_H
