/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 15:54:01 by amagno-r          #+#    #+#             */
/*   Updated: 2025/12/10 15:55:05 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdbool.h>
# include <stddef.h>

typedef enum e_token_type
{
	TOK_WORD = 1,
	TOK_ASSIGNMENT_WORD,
	TOK_PIPE,
	TOK_AND,
	TOK_OR,
	TOK_SEMI,
	TOK_AMP,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APPEND,
	TOK_REDIR_0_IN,
	TOK_REDIR_1_IN,
	TOK_REDIR_2_IN,
	TOK_REDIR_0_OUT,
	TOK_REDIR_1_OUT,
	TOK_REDIR_2_OUT,
	TOK_HEREDOC,
	TOK_OPEN_HEREDOC,
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_EOF
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*lexeme;
}					t_token;

typedef struct s_lexer
{
	const char		*input;
	size_t			input_len;
	size_t			position;
	size_t			read_position;
	char			ch;
	t_token			*tk;
}					t_lexer;

void				init_lexer(t_lexer *lx, char *input);
void				lexer_read_char(t_lexer *lexer);
char				lexer_peek_char(const t_lexer *lexer);
void				lexer_read_word(t_lexer *lexer, t_token *token);
void				lexer_skip_space(t_lexer *lexer);
t_token				*token_new(t_token_type type);
bool				lexer_next_dmeta(t_lexer *lexer);
bool				lexer_next_smeta(t_lexer *lexer);
bool				lexer_next_redir(t_lexer *lexer);
t_token				*lexer_next_token(t_lexer *lexer);
t_token_type		evaluate_assign(const t_token *t);
int					is_meta(char c);
int					is_break(char c);

#endif // MINISHELL_LEXER_H
