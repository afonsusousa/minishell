CC := gcc
CFLAGS := -Wall -Wextra -Werror -g
INCLUDES := -Iincludes -Ilib/libft
SRC_DIR := src
BUILD_DIR := build
LIBFT_DIR := lib/libft
LIBFT := $(LIBFT_DIR)/libft.a

SRCS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/prompt.c \
	$(SRC_DIR)/dispose.c \
	$(SRC_DIR)/ast/ast.c \
	$(SRC_DIR)/ast/ast_free.c \
	$(SRC_DIR)/ast/ast_list.c \
	$(SRC_DIR)/ast/ast_print.c \
	$(SRC_DIR)/lexer/lexer.c \
	$(SRC_DIR)/lexer/operations.c \
	$(SRC_DIR)/lexer/utils.c \
	$(SRC_DIR)/lexer/token_stream.c \
	$(SRC_DIR)/lexer/token_stream_api.c \
	$(SRC_DIR)/parser/parser.c \
	$(SRC_DIR)/parser/cores.c \
	$(SRC_DIR)/parser/logical.c \
	$(SRC_DIR)/parser/helpers.c \
	$(SRC_DIR)/parser/heredoc/capture.c \
	$(SRC_DIR)/parser/heredoc/descent.c \
	$(SRC_DIR)/parser/pipeline.c \
	$(SRC_DIR)/parser/redirs.c \
	$(SRC_DIR)/parser/utils.c \
	$(SRC_DIR)/parser/error.c \
	$(SRC_DIR)/env/utils.c \
	$(SRC_DIR)/env/getters.c \
	$(SRC_DIR)/env/setters.c \
	$(SRC_DIR)/env/api.c \
	$(SRC_DIR)/executor/executor.c \
	$(SRC_DIR)/executor/logical.c \
	$(SRC_DIR)/executor/redir/redirs.c \
	$(SRC_DIR)/executor/redir/utils.c \
	$(SRC_DIR)/executor/error.c \
	$(SRC_DIR)/executor/cores/cores.c \
	$(SRC_DIR)/executor/cores/builtin/builtins.c \
	$(SRC_DIR)/executor/cores/builtin/cd.c \
	$(SRC_DIR)/executor/cores/builtin/echo.c \
	$(SRC_DIR)/executor/cores/builtin/export.c \
	$(SRC_DIR)/executor/cores/builtin/env.c \
	$(SRC_DIR)/executor/cores/builtin/exit.c \
	$(SRC_DIR)/executor/cores/builtin/pwd.c \
	$(SRC_DIR)/executor/cores/builtin/unset.c \
	$(SRC_DIR)/executor/cores/command/command.c \
	$(SRC_DIR)/executor/cores/command/setup.c \
	$(SRC_DIR)/executor/cores/command/path_finding.c \
	$(SRC_DIR)/executor/cores/command/utils.c \
	$(SRC_DIR)/executor/pipeline/pipeline.c \
	$(SRC_DIR)/executor/pipeline/utils.c \
	$(SRC_DIR)/signal/signal.c \
	$(SRC_DIR)/subst/expansions.c \
	$(SRC_DIR)/subst/globbing.c \
	$(SRC_DIR)/subst/sm.c \
	$(SRC_DIR)/subst/sm_operations.c \
	$(SRC_DIR)/utils/sort.c \
	$(SRC_DIR)/utils/utils.c \
	$(SRC_DIR)/utils/word_split.c \
	$(SRC_DIR)/utils/word_utils.c

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
