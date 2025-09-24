#include <stddef.h>
#include <stdio.h>
#include "parser.h"


typedef struct s_parser {
    t_token_stream ts;     // token stream (array + current index)
} t_parser;

/*
	command_line ::= list command_line_endopt ;

	command_line_endopt ::= ";"
		| "&"
		| ε
		;
*/
static t_ast *parse_command_line(t_parser *p);

/*
	list ::= or_list ;
*/
static t_ast *parse_list(t_parser *p);

/*
Production:
	or_list ::= and_list or_list_tail ;

	or_list_tail ::= "||" and_list or_list_tail
		| ε
		;
*/
static t_ast *parse_or_list(t_parser *p);

/*
	and_list ::= pipeline and_list_tail ;

	and_list_tail ::= "&&" pipeline and_list_tail
		| ε
		;
*/
static t_ast *parse_and_list(t_parser *p);

/*
	pipeline ::= command pipeline_tail ;

	pipeline_tail ::= "|" command pipeline_tail
		| ε
		;
*/
static t_ast *parse_pipeline(t_parser *p);

/*
	command ::= command_core command_trailing_redirs ;

	command_core ::= grouping
		| simple_command
		;

	command_trailing_redirs ::= redirection command_trailing_redirs
		| ε
		;
*/
static t_ast *parse_command(t_parser *p);

/*
	command_core ::= grouping
		| simple_command
		;
*/
static t_ast *parse_command_core(t_parser *p);

/*
	simple_command ::= assignment_prefix_list simple_body_opt ;

	assignment_prefix_list ::= assignment_word assignment_prefix_list
		| ε
		;

	simple_body_opt ::= cmd_word_and_redirs
		| ε
		;

	cmd_word_and_redirs ::= cmd_item cmd_item_list ;

	cmd_item_list ::= cmd_item cmd_item_list
		| ε
		;

	cmd_item ::= word
		| redirection
		;
*/
static t_ast *parse_simple_command(t_parser *p);

/*
	redirection ::= redirection_op filename
		| "<<" word
		;
*/
static t_ast *parse_redirection(t_parser *p);

// command_line: Parse list then optional ';' or '&' terminator (store it) else ε.
static t_ast *parse_command_line(t_parser *p) { (void)p; return NULL; }
// list: Thin wrapper for or_list for future extensibility.
static t_ast *parse_list(t_parser *p) { (void)p; return NULL; }
// or_list: Left-associative chain of and_list joined by '||'.
static t_ast *parse_or_list(t_parser *p) { (void)p; return NULL; }
// and_list: Left-associative chain of pipeline joined by '&&'.
static t_ast *parse_and_list(t_parser *p) { (void)p; return NULL; }
// pipeline: Sequence of one or more command nodes separated by '|'.
static t_ast *parse_pipeline(t_parser *p) { (void)p; return NULL; }
// command: Core (grouping | simple_command) plus zero or more trailing redirections.
static t_ast *parse_command(t_parser *p) { (void)p; return NULL; }
// command_core: Distinguish grouping from simple_command based on '('.
static t_ast *parse_command_core(t_parser *p) { (void)p; return NULL; }
// simple_command: Leading assignment words then optional body: first cmd item + following items.
static t_ast *parse_simple_command(t_parser *p) { (void)p; return NULL; }
// redirection: Parse operator + filename OR heredoc operator + delimiter.
static t_ast *parse_redirection(t_parser *p) { (void)p; return NULL; }