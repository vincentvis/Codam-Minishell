SRCS := ../../source_files/builtins/echo.c \
		../../source_files/builtins/cd.c \
		../../source_files/builtins/export.c \
		../../source_files/builtins/unset.c \
		../../source_files/builtins/pwd.c \
		../../source_files/builtins/exit.c \
		../../source_files/builtins/env.c

NAME := builtins

include ../make_shared_among_tests.mk

TESTS := echo.c cd.c pwd.c export.c unset.c env.c exit.c

SRCS += $(FILES_FOR_ENV)
SRCS += $(FILES_FOR_EXEC)
SRCS += $(FILES_FOR_LEXER)
SRCS += $(FILES_FOR_PARSER)
SRCS += $(FILES_FOR_UTILS)
SRCS += $(FILES_FOR_EXPANSION)
SRCS += $(HELPERS)
