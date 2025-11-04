CC := gcc
CFLAGS := -Wall -Wextra -Werror -g
INCLUDES := -Iincludes -Ilib/libft
SRC_DIR := src
LIBFT_DIR := lib/libft
LIBFT := $(LIBFT_DIR)/libft.a

SRCS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/utils.c \
	$(SRC_DIR)/dispose.c \
	$(SRC_DIR)/ast/ast.c \
	$(SRC_DIR)/ast/ast_free.c \
	$(SRC_DIR)/ast/ast_list.c \
	$(SRC_DIR)/parse/lexer/lexer.c \
	$(SRC_DIR)/parse/lexer/token_stream.c \
	$(SRC_DIR)/parse/parser/parser.c \
	$(SRC_DIR)/env/envp.c \
	$(SRC_DIR)/executor/executor.c \
	$(SRC_DIR)/executor/binop.c \
	$(SRC_DIR)/executor/pipeline/pipeline.c \
	$(SRC_DIR)/executor/pipeline/cores/cores.c \
	$(SRC_DIR)/executor/pipeline/cores/builtin/builtins.c \
	$(SRC_DIR)/executor/pipeline/cores/builtin/export.c \
	$(SRC_DIR)/executor/pipeline/cores/command/command.c \
	$(SRC_DIR)/executor/pipeline/cores/command/command_setup.c \
	$(SRC_DIR)/executor/pipeline/cores/command/utils.c \
	$(SRC_DIR)/executor/pipeline/cores/core_redirs/core_redirs.c \
	$(SRC_DIR)/executor/pipeline/cores/core_redirs/heredoc.c \
	$(SRC_DIR)/subst/subst_machine.c \
	$(SRC_DIR)/subst/subst.c

OBJS := $(SRCS:.c=.o)
NAME := minishell

# Linker flags and libraries
LDFLAGS := -L$(LIBFT_DIR)
LDLIBS := -lft -lreadline -lncurses

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
