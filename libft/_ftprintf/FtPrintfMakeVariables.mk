FTPRINTF_SRC = ft_printf.c ft_vprintf.c \
		parse/build_list.c \
		parse/fmt.c parse/fmt_undefined.c parse/fmt_type_n.c \
		parse/fmt_type_nbr.c parse/fmt_type_str.c parse/fmt_get_arg.c \
		parse/fmt_type_f.c parse/ft_utf8.c \
		helpers/jumptables_fmt.c \
		process/process_list.c process/string.c process/nbr.c process/double.c

LIBFT_SRC += $(addprefix _ftprintf/,$(FTPRINTF_SRC))

LIBFT_OBJ = $(addprefix objs/, $(LIBFT_SRC:.c=.o))
