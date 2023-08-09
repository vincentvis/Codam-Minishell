SRCS := ../../source_files/parser/ast.c \
		../../source_files/parser/command.c \
		../../source_files/parser/io_redirect.c \
		../../source_files/parser/node.c \
		../../source_files/parser/pipe_sequence.c \
		../../source_files/parser/utils.c \
		../../source_files/parser/validation.c

NAME := parser

include ../make_shared_among_tests.mk

SRCS += $(FILES_FOR_UTILS)
SRCS += $(FILES_FOR_ENV)
SRCS += $(FILES_FOR_LEXER)
TESTS += parser_invalid_input.c
HELPERS += parser_helpers.c
SRCS += $(HELPERS)
