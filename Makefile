CC := gcc
CFLAGS := -Wall -Wextra -Werror -g -lreadline -lncurses
INCLUDES := -Iincludes -Ilib/libft
SRC_DIR := src
LIBFT_DIR := lib/libft
LIBFT := $(LIBFT_DIR)/libft.a

SRCS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/lexer.c \
	$(SRC_DIR)/token_stream.c \
	$(SRC_DIR)/ast.c \
	$(SRC_DIR)/ast_free.c \
	$(SRC_DIR)/ast_list.c \
	$(SRC_DIR)/parser.c \
	$(SRC_DIR)/envp.c \
	$(SRC_DIR)/executor.c \
	$(SRC_DIR)/dispose.c \
	$(SRC_DIR)/heredoc.c \
	$(SRC_DIR)/subst.c

OBJS := $(SRCS:.c=.o)
NAME := minishell

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -L$(LIBFT_DIR) -lft

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
