SRCS := ../../source_files/expansion/expand_ifs_find_pos.c \
		../../source_files/expansion/expand_ifs_helpers.c \
		../../source_files/expansion/expand_ifs_tokens.c \
		../../source_files/expansion/expand_ifs.c \
		../../source_files/expansion/expand_validation.c \
		../../source_files/expansion/expansion.c \
		../../source_files/expansion/transition_tables.c \
		../../source_files/expansion/type/dquote.c \
		../../source_files/expansion/type/exitcode.c \
		../../source_files/expansion/type/simple_quotes.c \
		../../source_files/expansion/type/tilde.c \
		../../source_files/expansion/type/var.c \
		../../source_files/expansion/type/word.c \
		../../source_files/expansion/update_tree.c

NAME := expansion

include ../make_shared_among_tests.mk

HELPERS += expansion_helpers.c

SRCS += $(FILES_FOR_BUILTINS)
SRCS += $(FILES_FOR_ENV)
SRCS += $(FILES_FOR_EXEC)
SRCS += $(FILES_FOR_LEXER)
SRCS += $(FILES_FOR_PARSER)
SRCS += $(FILES_FOR_UTILS)
SRCS += $(HELPERS)

EXTRA_SRCS := $(filter-out ../../source_files/expansion/expansion.c,$(SRCS))

extra:
	@$(CC) $(CFLAGS) $(LDLIBS) $(EXTRA_SRCS) expansion_static_tilde_test.c $(CPPFLAGS) $(LIBFT) -o static_tilde.out
	@(./static_tilde.out && echo "Success 'expansion static_tilde' tests") || (echo "exit 2" >> ../done)
ifeq ($(MAKELEVEL), 0)
	@rm -f ../done
endif
