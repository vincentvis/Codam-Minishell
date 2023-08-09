/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/04 14:04:56 by jowytrzy      #+#    #+#                 */
/*   Updated: 2021/05/09 15:44:17 by jowytrzy      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

static void	copy_and_delete(char *line, t_line *chunk)
{
	gnl_strlcpy_charptr(line, chunk->line, chunk->length);
	line += chunk->length;
	if ((chunk->next) && chunk->line[chunk->length] != '\n')
		copy_and_delete(line, chunk->next);
	else if (chunk->line[chunk->length] == '\n')
		*line = '\n';
	else
		*line = '\0';
	free(chunk->line);
	free(chunk);
}

static int	patch_lines(t_list_gnl *current)
{
	t_line	*line;

	if (current->next_line->line[current->next_line->length] == '\n'
		|| !(current->next_line->next))
		return (0);
	line = (t_line *)malloc(sizeof(t_line));
	if (!line)
		return (-1);
	gnl_make_long_line(current->next_line, line);
	line->line = (char *)malloc(sizeof(char) * (line->length + 1));
	if (line->line == NULL)
		return (-1);
	copy_and_delete(line->line, current->next_line);
	current->next_line = line;
	return (0);
}

static int	split_lines(char *buffer, t_line **new_line)
{
	*new_line = malloc(sizeof(t_line));
	if (!(*new_line))
		return (-1);
	(*new_line)->next = NULL;
	(*new_line)->length = 0;
	while (buffer[(*new_line)->length] != '\0'
		&& buffer[(*new_line)->length] != '\n')
		(*new_line)->length += 1;
	(*new_line)->line = malloc(sizeof(char) * ((*new_line)->length + 1));
	if (!(*new_line)->line)
		return (-1);
	gnl_strlcpy_charptr((*new_line)->line, buffer, (*new_line)->length + 1);
	buffer = &buffer[(*new_line)->length];
	if (*buffer == '\n')
		buffer++;
	if (*buffer != '\0')
		if (split_lines(buffer, &((*new_line)->next)) == -1)
			return (-1);
	if ((*new_line)->line[(*new_line)->length] == '\n')
		return (1);
	return (0);
}

static int	read_file(t_list_gnl **head, t_list_gnl *current)
{
	ssize_t	read_bytes;
	t_line	**new_line;
	char	*buffer;
	int		nl;

	nl = 0;
	read_bytes = BUFFER_SIZE;
	new_line = &current->next_line;
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	while (!nl && read_bytes == BUFFER_SIZE)
	{
		read_bytes = read(current->fd, buffer, BUFFER_SIZE);
		if (read_bytes == -1)
			return (gnl_free_all(head, current, current->next_line));
		buffer[read_bytes] = '\0';
		if (*new_line != NULL)
			new_line = gnl_append_here(*new_line);
		nl = split_lines(buffer, new_line);
		if (nl == -1)
			return (gnl_free_all(head, current, current->next_line));
	}
	free(buffer);
	return (patch_lines(current));
}

int	get_next_line(int fd, char **line)
{
	static t_list_gnl	*head;
	t_list_gnl			*current;
	t_line				*tmp;
	int					ret;

	if (fd < 0 || !line || BUFFER_SIZE < 1 || read(fd, NULL, 0) == -1)
		return (-1);
	current = gnl_find_add_fd(&head, fd);
	if (!current || ((!(current->next_line)
				|| current->next_line->line[current->next_line->length] != '\n')
			&& read_file(&head, current) == -1))
		return (-1);
	*line = current->next_line->line;
	tmp = current->next_line;
	ret = 0;
	if ((*line)[current->next_line->length] == '\n')
	{
		ret = 1;
		(*line)[current->next_line->length] = '\0';
	}
	current->next_line = current->next_line->next;
	if (current->next_line == NULL && ret == 0)
		gnl_free_all(&head, current, NULL);
	free(tmp);
	return (ret);
}
