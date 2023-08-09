# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    GnlMakeVariables                                   :+:    :+:             #
#                                                      +:+                     #
#    By: vincent <vincent@student.codam.nl>           +#+                      #
#                                                    +#+                       #
#    Created: 2020/09/17 20:48:20 by vincent       #+#    #+#                  #
#    Updated: 2020/09/18 10:08:08 by vincent       ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

GNL_SRC +=	get_next_line.c \
				get_next_line_utils.c

LIBFT_SRC += $(addprefix _gnl/,$(GNL_SRC))

LIBFT_OBJ = $(addprefix objs/, $(LIBFT_SRC:.c=.o))
