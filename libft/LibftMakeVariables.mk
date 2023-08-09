# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    LibftMakeVariables.mk                              :+:    :+:             #
#                                                      +:+                     #
#    By: vincent <vincent@student.codam.nl>           +#+                      #
#                                                    +#+                       #
#    Created: 2020/05/12 14:05:02 by vvissche      #+#    #+#                  #
#    Updated: 2022/02/17 00:41:24 by jwytrzyszcz   ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

LIBFT_INCLUDE_DIR = include/

LIBFT_HEADER_DEPS = libft.h ft_nbr.h ft_bigint.h
LIBFT_HEADERS = $(addprefix $(LIBFT_INCLUDE_DIR),$(LIBFT_HEADER_DEPS))

LIBFT_SRC =	helpers/ft_segdefault.c helpers/free_char_pointer_array.c is/ft_is_null.c \
		is/ft_isalnum.c is/ft_isalpha.c is/ft_isascii.c is/ft_isdigit.c \
		is/ft_islower.c is/ft_isprint.c is/ft_isspace.c is/ft_isupper.c \
		list/ft_lstadd_back.c list/ft_lstadd_front.c list/ft_lstclear.c \
		list/ft_lstdelone.c list/ft_lstiter.c list/ft_lstlast.c \
		list/ft_lstmap.c list/ft_lstnew.c list/ft_lstsize.c \
		mem/ft_bzero.c mem/ft_calloc.c mem/ft_memccpy.c mem/ft_memchr.c \
		mem/ft_memcmp.c mem/ft_memcpy.c mem/ft_memdup.c mem/ft_memmove.c \
		mem/ft_memset.c mem/ft_memset_int.c \
		nbr/ft_atoi.c nbr/ft_strtol.c nbr/ft_strtoll.c \
		nbr/ft_nbrlen.c nbr/ft_nbrlen_unsigned.c nbr/ft_itoa.c \
		nbr/ft_nbrtostr.c nbr/ft_nbrtostr_base.c nbr/ft_nbrtostr_base16.c \
		nbr/ft_nbrtostr_base2.c nbr/ft_nbr_base_strings.c \
		bigint/ft_bigint.c bigint/ft_bigint_from.c \
		bigint/ft_bigint_add.c bigint/ft_bigint_div.c bigint/ft_bigint_mul.c \
		bigint/ft_bigint_double.c \
		string/ft_split.c string/ft_strchr.c string/ft_strdup.c \
		string/ft_strjoin.c string/ft_strlcat.c string/ft_strlcpy.c \
		string/ft_strlen.c string/ft_wcslen.c \
		string/ft_strnlen.c string/ft_wcsnlen.c \
		string/ft_strmapi.c string/ft_strncmp.c string/ft_strnstr.c \
		string/ft_strrchr.c string/ft_strtrim.c string/ft_substr.c \
		to/ft_tolower.c to/ft_toupper.c \
		write/ft_putchar_fd.c write/ft_putendl_fd.c write/ft_putnbr_fd.c \
		write/ft_putstr_fd.c

LIBFT_OBJ = $(addprefix objs/, $(LIBFT_SRC:.c=.o))
