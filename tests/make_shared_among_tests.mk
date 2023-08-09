LDLIBS := -lcriterion
LIBFT := ../../libft/libft.a
CPPFLAGS := -I../../include  -I../../libft/include
CFLAGS := -Wall -Werror -Wextra

FILES_FOR_ENV := ../../source_files/env/*.c
FILES_FOR_LEXER := ../../source_files/lexer/*.c
FILES_FOR_PARSER := ../../source_files/parser/*.c
FILES_FOR_UTILS := ../../source_files/utils/*.c
FILES_FOR_EXPANSION := ../../source_files/expansion/*.c
FILES_FOR_EXPANSION += ../../source_files/expansion/type/*.c
FILES_FOR_EXEC := ../../source_files/executor/*.c
FILES_FOR_EXEC += ../../source_files/executor/heredoc/*.c
FILES_FOR_BUILTINS := ../../source_files/builtins/*.c

ifeq ($(CRITERION_BREW_DIR),)
CRITERION_BREW_DIR := $(shell brew --prefix criterion)
endif

ifeq ($(READLINE_DIR),)
include ../make_get_readline_dir.mk
endif

LDLIBS += -L$(CRITERION_BREW_DIR)/lib -L$(READLINE_DIR)/lib -lreadline
CPPFLAGS += -I$(CRITERION_BREW_DIR)/include -I$(READLINE_DIR)/include

TESTS := $(NAME).c
HELPERS := ../generic_test_helpers.c
OUTFILE := $(NAME).out

ALLOC_DEBUGGING := -D DO_ALLOC_FAILING

.PHONY: create
create:
	@$(CC) $(CFLAGS) $(LDLIBS) $(TESTS) $(SRCS) $(CPPFLAGS) $(LIBFT) $(ALLOC_DEBUGGING) -o $(OUTFILE)

.PHONY: run
run: create
	@$(MAKE) -s extra
	@(./$(OUTFILE) && echo "Success '$(NAME)' tests") || (echo "exit 2" >> ../done)
ifeq ($(MAKELEVEL), 0)
	@rm -f ../done
endif

.PHONY: extra
extra:
