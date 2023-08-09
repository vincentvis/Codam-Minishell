/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/04 14:04:45 by jowytrzy      #+#    #+#                 */
/*   Updated: 2021/05/09 15:44:57 by jowytrzy      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_list_gnl
{
	int					fd;
	struct s_line		*next_line;
	struct s_list_gnl	*next;
}						t_list_gnl;

typedef struct s_line
{
	char			*line;
	int				length;
	struct s_line	*next;
}					t_line;

int					get_next_line(int fd, char **line);
char				*gnl_strlcpy_charptr(char *dst, const char *src,
						size_t dstsize);
t_line				**gnl_append_here(t_line *lst);
void				gnl_make_long_line(t_line *chunk, t_line *long_line);
t_list_gnl			*gnl_find_add_fd(t_list_gnl **head, int fd);
int					gnl_free_all(t_list_gnl **head, t_list_gnl *current,
						t_line *first_line);

#endif
