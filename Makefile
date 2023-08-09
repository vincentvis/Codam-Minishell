
# Project name and name of final executable
NAME = minishell

# enable(1) when debugging
ifndef DEBUGGING
	DEBUGGING = 1
endif

CALL_TREE_FILE := ./source_files/debug/call_tree/CALL_TREE_JSON.json
NM_FILE := ./source_files/debug/call_tree/NM_OUT.log
ifndef CALL_TREE
    CALL_TREE := 0
endif

# Override some default/implicit MAKE variables
# Extra flags to give to the C compiler
CFLAGS := -Wall -Werror -Wextra
ifeq ($(DEBUGGING), 1)
	CFLAGS += -g -pedantic -O0
endif

ifeq ($(CALL_TREE), 1)
    CFLAGS += -finstrument-functions -fno-pie
    CC := gcc
endif

# check for homebrew and if homebrew exists
# check for the readline path
ifneq (,$(strip $(shell which brew)))
READLINE_DIR := $(shell brew --prefix readline)
endif

# check if readline exists at the brew location
# this also captures if there was no brew found
ifeq (,$(wildcard $(READLINE_DIR)))
	ifneq (,$(wildcard /usr/local/opt/readline))
		READLINE_DIR := /usr/local/opt/readline
	else
		PLZ_INSTALL_MSG := Please install with `brew install readline`
		READLINE_DIR := $(error Could not find readline! $(PLZ_INSTALL_MSG))
	endif
endif

# Extra flags to give to the compiler when invoking the Linker (`ld`)
LDFLAGS := -L$(READLINE_DIR)/lib -Llibft
# Library flags or names given to the compiler when invoking the linker (`ld`)
LDLIBS := -lft -lreadline
# Extra flags to give to the C preprocessor (for including headers)
CPPFLAGS := -I./include -I./libft/include -I$(READLINE_DIR)/include

# Headerfiles needed as dependencies for the project
HEADER_DIR := include/
HEADERS :=  builtins/builtins.h \
			builtins/builtins_internal.h \
			env/she_env.h \
			env/she_env_internal.h \
			executor/exec_internal.h \
			executor/execute.h \
			executor/heredoc.h \
			expansion/expand_functions.h \
			expansion/expand_validation.h \
			expansion/expansion.h \
			expansion/transition_tables.h \
			lexer/lexer.h \
			lexer/lexer_private.h \
			lexer/state.h \
			lexer/token.h \
			lexer/validation.h \
			parser/parser.h \
			parser/validation.h \
			utils/exitcode.h \
			utils/ifs.h \
			utils/interactive.h \
			utils/minishell_colors.h \
			utils/readline.h \
			utils/signals.h \
			utils/status.h \
			utils/vector.h

DEBUG_HEADERS := debug/minishell_debug.nonorm
ifeq ($(DEBUGGING), 1)
    HEADERS += $(DEBUG_HEADERS)
endif

HEADERS := 	$(addprefix $(HEADER_DIR), $(HEADERS))

# location of libft library
LIBFT := libft/libft.a

# the directory that contains the .c files
SOURCE_DIR := source_files/
# source files for the project
# without main source_file prefix (added later for shorter list here)
SOURCE_FILES :=	minishell.c \
				lexer/lexer.c \
				lexer/review_tokens.c \
				lexer/token.c \
				lexer/validation_compound_words.c \
				lexer/validation_symbols.c \
				lexer/validation_var.c \
				lexer/validation_words.c \
				parser/ast.c \
				parser/command.c \
				parser/io_redirect.c \
				parser/node.c \
				parser/pipe_sequence.c \
				parser/utils.c \
				parser/validation.c \
				env/she_env.c \
				env/she_env_create.c \
				env/she_env_get.c \
				env/she_env_helpers.c \
				env/she_env_init.c \
				env/she_env_misc.c \
				env/she_env_sort.c \
				env/she_env_store.c \
				env/she_env_unset.c \
				env/she_env_update.c \
				env/she_env_var.c \
				executor/builtins.c \
				executor/collect_arguments.c \
				executor/execute.c \
				executor/execute_command.c \
				executor/execute_pipe_sequence.c \
				executor/files.c \
				executor/heredoc/heredoc.c \
				executor/heredoc/heredoc_input.c \
				executor/path.c \
				executor/processes.c \
				executor/redirections.c \
				expansion/expand_ifs.c \
				expansion/expand_ifs_find_pos.c \
				expansion/expand_ifs_helpers.c \
				expansion/expand_ifs_tokens.c \
				expansion/expand_validation.c \
				expansion/expansion.c \
				expansion/transition_tables.c \
				expansion/type/dquote.c \
				expansion/type/exitcode.c \
				expansion/type/simple_quotes.c \
				expansion/type/tilde.c \
				expansion/type/var.c \
				expansion/type/word.c \
				expansion/update_tree.c \
				builtins/cd.c \
				builtins/echo.c \
				builtins/env.c \
				builtins/exit.c \
				builtins/export.c \
				builtins/pwd.c \
				builtins/unset.c \
				utils/error_handlers.c \
				utils/exit_errors.c \
				utils/exitcode.c \
				utils/ifs.c \
				utils/interactive.c \
				utils/readline.c \
				utils/signal_handlers.c \
				utils/signals.c \
				utils/status.c \
				utils/vector.c \
				utils/vector_peek.c

DEBUG_FILES := debug/print_information.c \
                debug/generate_ast_dotfile.c \
                debug/enum_value_to_string.c

ifeq ($(DEBUGGING), 1)
    SOURCE_FILES += $(DEBUG_FILES)
endif

ifeq ($(CALL_TREE),1)
    SOURCE_FILES += debug/call_tree/call_tree.c
endif

# addprefix source_files/ to sources
SRC := $(addprefix $(SOURCE_DIR), $(SOURCE_FILES))

# directory that will contain the .o files
OBJECT_DIR := object_files/
# Variable with object files put in an object_files subdirectory
OBJ := $(subst $(SOURCE_DIR),$(OBJECT_DIR), $(SRC:.c=.o))

# NORM required rule
# Just compiles the project
all: $(NAME)

# compiles .c files to .o files
# put them all in a object_files directory to make it cleaner
$(OBJECT_DIR)%.o: $(SOURCE_DIR)%.c $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# NORM required rule
# compiles whole project
$(NAME): $(LIBFT) $(OBJ) $(HEADERS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) $(OBJ) -o $(NAME)

# NORM required rule
# Cleans files generated during compilation but that are not the final program
clean:
	$(RM) -r $(OBJECT_DIR)
	@$(MAKE) -C libft clean

# NORM required rule
# Cleans all files and leaves only the original, uncompiled source files behind
fclean: clean
	$(RM) $(NAME)
	@$(MAKE) -C libft fclean
	$(RM) $(NM_FILE)
	$(RM) $(CALL_TREE_FILE)

softclean:
	$(RM) -r $(OBJECT_DIR)
	$(RM) $(NAME)

calltree: softclean
	@$(MAKE) CALL_TREE=1
	nm -U $(NAME) > $(NM_FILE)

# NORM required rule
# `re`compiles the project
re: fclean all

# compiles libft
$(LIBFT):
	$(MAKE) -C $(dir $(LIBFT)) all

# A phony target is one that is not really the name of a file; rather it is just
# a name for a recipe to be execured when you make an explicit request.
.PHONY: clean fclean re all
