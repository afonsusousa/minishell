#ifndef MINISHELL_PARSER_H
#define MINISHELL_PARSER_H

#include <stddef.h>
#include "tokens.h"
#include "ast.h"

// Public parser entrypoint (may be implemented later)
t_ast *parse(const t_token *tokens, size_t count);

#endif // MINISHELL_PARSER_H
