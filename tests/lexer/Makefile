SRCS := ../../source_files/lexer/lexer.c \
		../../source_files/lexer/token.c \
		../../source_files/lexer/review_tokens.c \
		../../source_files/lexer/validation_words.c \
		../../source_files/lexer/validation_compound_words.c \
		../../source_files/lexer/validation_var.c \
		../../source_files/lexer/validation_symbols.c

NAME := lexer

include ../make_shared_among_tests.mk

SRCS += $(FILES_FOR_ENV)
SRCS += $(FILES_FOR_UTILS)
TESTS += validation.c token.c
