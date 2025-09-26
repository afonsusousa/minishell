CC := gcc
CFLAGS := -Wall -Wextra -Werror -g
INCLUDES := -Iincludes
SRC_DIR := src

SRCS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/lexer.c \
	$(SRC_DIR)/token_stream.c \
	$(SRC_DIR)/ast.c \
	$(SRC_DIR)/ast_free.c \
	$(SRC_DIR)/ast_list.c \
	$(SRC_DIR)/parser.c

OBJS := $(SRCS:.c=.o)
NAME := minishell_demo

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
