*This project has been created as part of the 42 curriculum by amagno-r, wlucas-f.*

# Minishell

## Description
Minishell is a simple shell implementation inspired by bash. The goal of this project was to mimic bash's behaviour as close as possible, within a limited set of its features.

## Instructions

### Compilation
To compile the project, simply run `make` in the root directory. This will generate the `minishell` executable.

```bash
make
```

### Execution
To start the shell, run the executable:

```bash
./minishell
```

You can also execute commands directly using the `-c` flag:

```bash
./minishell -c "ls -l"
```

## Resources
- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) - The official documentation for bash, used as a reference for behavior.
- [BNF Grammars](https://cs61.seas.harvard.edu/site/2021/BNFGrammars/) - The left-recursive grammar from which our right-rec one was derived for the parser.
- [Lexer](https://github.com/ThePrimeagen/ts-rust-zig-deez) - Non-ironically the lexer was based on the Holy-C's entry for Primeagen's lexer competition.
- [Grammars, parsing, and recursive descent](https://www.youtube.com/watch?v=ENKT0Z3gldE&t=1302s) - Youtuber Kay Lack's amazing video on recursive descent and why a left-recursive grammar wouldn't fit our needs. Strongly recommend anyone to nerd out on this channel.

## Features
- **Prompt**: Displays a prompt when waiting for a new command, mimicking bash's.
- **Logical Operators**: Supports `&&` and `||` for conditional execution of commands.
- **Pipes**: Allows chaining commands using pipes (`|`).
- **Redirections**: Supports standard input (`<`), output (`>`), append (`>>`), and heredoc (`<<`) redirections. It also explicitly handles file descriptor redirections for stdin, stdout, and stderr (e.g., `2>`, `1<`, `0<<`, etc.).
- **Environment Variables**: Manages environment variables and supports expansion (`$VAR`).
- **Wildcards**: Supports the `*` wildcard for filename expansion in the current directory.
- **Subshells**: Supports grouping commands using parentheses `( ... )` to execute them in a subshell.
- **Signals**: Handles signals like `SIGINT` (Ctrl+C), `SIGQUIT` (Ctrl+\), and `SIGEOF` (Ctrl+D) appropriately.
- **Built-ins**: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit` are provided with a minimal feature set.
- **Assignments**: Simple assignments that can be ran either in combination with commands or standalone `mininshell> a="Hello, World!"`. These are only valid for the children's env and do not affect the running env. 

## Under the Hood

### Lexer
A lexer which builds a simple array of tokens, that are either shell symbols `|, >>, &&, ||` or words - quoted or unquoted.
For the purposes of this project, the lexer is simply aware of quote opening and closing, but does nothing to enforce balance or correctness.

The lexer is structured around a `t_lexer` struct that maintains the current state of the input string, including the current position, read position, and the character under examination. Helper functions like `lexer_read_char`, `lexer_peek_char`, and specific handlers for redirections (`lexer_next_redir`) and meta-characters (`lexer_next_dmeta`, `lexer_next_smeta`) are used to tokenize the input efficiently. This modular approach allows for easy extension and maintenance of the tokenization logic.

### Parser
For the parser, we opted to define a right recursive BNF grammar that allowed us to implement a Recursive Descent Parser.
This decision greatly improved our development experience as both the Parser and Executor follow a strict set of rules that help ensure the AST
is correct. This is to say: "if the grammar is sane, the parser, AST and executor are sane". Having this strict grammar as a base helped us not run into any major surprises
and, as we consider it, the Parser is a 1:1 "translation" of its BNF counterpart.

To aid in the parsing process, and inspired by the quality of life abstractions found in the Lexer, we implemented a `t_token_stream` structure. This structure acts as a wrapper around the token list and provides helper functions like `ts_peek`, `ts_next`, and `ts_match`, allowing the parser to easily inspect and consume tokens without manually managing the list traversal. This abstraction simplified the parser logic significantly, making the code more readable and maintainable.

```bnf
command_line ::= list command_line_endopt ;

command_line_endopt ::= ";"
                     | "&"
                     | ε
                     ;

list ::= pipeline list_tail ;

list_tail ::= "&&" pipeline list_tail
            | "||" pipeline list_tail
            | ε
            ;

//  PIPELINE
pipeline ::= core pipeline_tail ;

//      TAIL
pipeline_tail ::= "|" core pipeline_tail
                | ε
                ;
core ::= command
       | grouping
       ;

//  COMMAND
command ::= assignments_list cmd_items ;

cmd_items ::= cmd_item cmd_items
            | ε
            ;
cmd_item ::= word
           | redirection
           ;

// GROUPING (subshell)
grouping ::= "(" list ")" grouping_redirs ;

grouping_redirs ::= redirection grouping_redirs
                      | ε
                      ;

// Redirections (add IO_NUMBER variants?)
redirection ::= redirection_op WORD
              ;

redirection_op ::= "<"
                 | ">"
                 | ">>"
                 | "<<"
                 ;

assignments_list ::= assignment_word assignments_list
                   | ε
                   ;

assignment_word ::= ASSIGNMENT_WORD
                  | APPEND_WORD
                  ;

word ::= WORD
       | ASSIGNMENT_WORD
       | APPEND_WORD
       ;
```

### Executor
The Executor, on the other hand, was where the most shenanigans were encountered, especially when it came to substitutions and expansions, which are not as trivial as one would expect at
face value. A lot of work went into this, from the development of a state machine to handle expansions, to the implementation of a t_word type, mainly to compensate for the lexer's simplicity 
in defining a "word" On the other hand, the executor was a "reversal" of the parser, which made it somewhat easy to implement, execution logic-wise. 

#### Expansions
To handle variable expansions and quote removal correctly, we implemented a state machine. This machine transitions between different "character consumption" states (DEFAULT, IN_SQ, IN_DQ, IN_VAR), allowing us to accurately identify which parts of a string should be expanded and which should remain literal.

The state machine generates a `t_word` structure, which contains not only the expanded string but also a map indicating which characters were originally quoted. This information is crucial for subsequent steps, such as word splitting and wildcard expansion, as it allows us to distinguish between literal characters and those that should be subject to further processing.

```
     +-----------+       +-------+
     |           |------>|       |
---> |  DEFAULT  |   '   | IN_SQ |
     |           |<------|       |
     +-----------+       +-------+
       |   ^   ^ |   $
       |   |   | +------->+--------+
       | " |   +----------| IN_VAR |
       |   |      ^EOVAR  +--------+
       v   |              ^  |
     +-------+      $     |  |
     |       |------------+  |
     | IN_DQ |<--------------+
     |       |     ^EOVAR
     +-------+
```

### Project Structure
```
.
├── includes
│   ├── ast.h
│   ├── envp.h
│   ├── executor.h
│   ├── globbing.h
│   ├── lexer.h
│   ├── minishell.h
│   ├── parser.h
│   ├── sig.h
│   ├── tokens.h
│   └── utils.h
└─ src
    ├── dispose.c
    ├── main.c
    ├── prompt.c
    ├── ast
    │   ├── ast.c
    │   ├── ast_free.c
    │   ├── ast_list.c
    │   ├── ast_nodes_branches.c
    │   └── ast_nodes_leafs.c
    ├── env
    │   ├── api.c
    │   ├── getters.c
    │   ├── setters.c
    │   └── utils.c
    ├── executor
    │   ├── error.c
    │   ├── executor.c
    │   ├── logical.c
    │   ├── cores
    │   │   ├── cores.c
    │   │   ├── builtin
    │   │   │   ├── builtins.c
    │   │   │   ├── builtins.h
    │   │   │   ├── cd.c
    │   │   │   ├── echo.c
    │   │   │   ├── env.c
    │   │   │   ├── exit.c
    │   │   │   ├── export.c
    │   │   │   ├── export_error.c
    │   │   │   ├── pwd.c
    │   │   │   └── unset.c
    │   │   └── command
    │   │   │   ├── command.c
    │   │   │   ├── path_finding.c
    │   │   │   ├── setup.c
    │   │   │   └── utils.c
    │   ├── pipeline
    │   │   ├── pipeline.c
    │   │   └── utils.c
    │   └── redir
    │       ├── redirs.c
    │       └── utils.c
    ├── lexer
    │   ├── lexer.c
    │   ├── operations.c
    │   ├── token_stream.c
    │   ├── token_stream_api.c
    │   └── utils.c
    ├── parser
    │   ├── abort.c
    │   ├── cores.c
    │   ├── error.c
    │   ├── logical.c
    │   ├── parser.c
    │   ├── pipeline.c
    │   ├── redirs.c
    │   ├── utils.c
    │   └── heredoc
    │       ├── capture.c
    │       └── descent.c
    ├── signal
    │   └── signal.c
    ├── subst
    │   ├── expansions.c
    │   ├── globbing.c
    │   ├── sm.c
    │   ├── sm_init.c
    │   ├── sm_operations.c
    │   ├── word_array_utils.c
    │   ├── word_cstr_utils.c
    │   ├── word_split.c
    │   └── word_utils.c
    └── utils
        ├── arrays.c
        ├── libftpp.c
        ├── predicate.c
        └── sort.c
```

### AI Usage
AI tools were used to assist in the following tasks:
- **Code Optimization**: Suggestions for optimizing data structures and algorithms were provided by AI.
- **Documentation**: AI assisted in drafting and refining the documentation, including this README.
