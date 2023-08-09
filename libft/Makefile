# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: vincent <vincent@student.codam.nl>           +#+                      #
#                                                    +#+                       #
#    Created: 2019/10/29 09:29:39 by vvissche      #+#    #+#                  #
#    Updated: 2021/03/24 10:59:44 by vincent       ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# include LIBFT sources
include LibftMakeVariables.mk

# include ft_printf by default
ifndef PRINTF
	include _ftprintf/FtPrintfMakeVariables.mk
else
	# if PRINTF is supplied as an argument, and is 1, add printf
	# if PRINTF is not needed, do make PRINTF=0
	ifeq ($(PRINTF), 1)
		include _ftprintf/FtPrintfMakeVariables.mk
	endif
endif

# include get_next_line by default
ifndef GNL
	include _gnl/GnlMakeVariables.mk
else
	# if GNL is supplied as an argument, and is 1, add gnl
	# if GNL is not needed, do make GNL=0
	ifeq ($(GNL), 1)
		include _gnl/GnlMakeVariables.mk
	endif
endif

NAME = libft.a

CFLAGS = -Wall -Wextra -Werror -I$(LIBFT_INCLUDE_DIR)

all: $(NAME)

$(NAME): $(LIBFT_HEADERS) $(LIBFT_OBJ)
	ar -rcs $(NAME) $(LIBFT_OBJ)

objs/%.o: %.c
	$(shell mkdir -p $(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

LIBFT_CLEAN += $(LIBFT_OBJ)

LIBFT_FCLEAN += $(NAME) a.out

clean:
	$(RM) $(LIBFT_CLEAN)
	$(RM) -rf objs/

fclean: clean
	$(RM) $(LIBFT_FCLEAN)

re: fclean all

.PHONY: clean fclean re all
