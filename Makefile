CC := gcc
CFLAGS := -Wall -Wextra -Werror -g
INCLUDES := -Iincludes -Ilib/libft
SRC_DIR := src
BUILD_DIR := build
LIBFT_DIR := lib/libft
LIBFT := $(LIBFT_DIR)/libft.a

SRCS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/utils.c \
	$(SRC_DIR)/dispose.c \
	$(SRC_DIR)/ast/ast.c \
	$(SRC_DIR)/ast/ast_free.c \
	$(SRC_DIR)/ast/ast_list.c \
	$(SRC_DIR)/ast/ast_print.c \
	$(SRC_DIR)/lexer/lexer.c \
	$(SRC_DIR)/lexer/token_stream.c \
	$(SRC_DIR)/parser/parser.c \
	$(SRC_DIR)/parser/cores.c \
	$(SRC_DIR)/parser/logical.c \
	$(SRC_DIR)/parser/helpers.c \
	$(SRC_DIR)/parser/heredoc.c \
	$(SRC_DIR)/parser/pipeline.c \
	$(SRC_DIR)/parser/redirs.c \
	$(SRC_DIR)/parser/utils.c \
	$(SRC_DIR)/parser/error.c \
	$(SRC_DIR)/env/envp.c \
	$(SRC_DIR)/executor/executor.c \
	$(SRC_DIR)/executor/logical.c \
	$(SRC_DIR)/executor/pipeline.c \
	$(SRC_DIR)/executor/cores.c \
	$(SRC_DIR)/executor/redirs.c \
	$(SRC_DIR)/executor/cores/builtin/builtins.c \
	$(SRC_DIR)/executor/cores/builtin/export.c \
	$(SRC_DIR)/executor/cores/command/command.c \
	$(SRC_DIR)/executor/cores/command/setup.c \
	$(SRC_DIR)/executor/cores/command/utils.c \
	$(SRC_DIR)/signal/signal.c \
	$(SRC_DIR)/subst/subst_machine.c \
	$(SRC_DIR)/subst/globbing.c

OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

NAME := minishell

LDFLAGS := -L$(LIBFT_DIR)
LDLIBS := -lft -lreadline -lncurses

CFLAGS += -MMD -MP

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
