/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_utils.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/04 14:04:53 by jowytrzy      #+#    #+#                 */
/*   Updated: 2021/05/09 15:44:18 by jowytrzy      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>

char	*gnl_strlcpy_charptr(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (!src || !dst)
		return (0);
	while (i < dstsize && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	if (dstsize - i >= 1)
		dst[i] = '\0';
	return (&dst[i]);
}

t_line	**gnl_append_here(t_line *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (&lst->next);
}

void	gnl_make_long_line(t_line *chunk, t_line *full_line)
{
	full_line->length = 0;
	while (chunk != NULL && chunk->line[chunk->length] != '\n')
	{
		full_line->length += chunk->length;
		chunk = chunk->next;
	}
	if (chunk && chunk->line[chunk->length] == '\n')
	{
		full_line->length += chunk->length;
		chunk = chunk->next;
	}
	full_line->next = chunk;
}

t_list_gnl	*gnl_find_add_fd(t_list_gnl **head, int fd)
{
	t_list_gnl	*new;
	t_list_gnl	*temp;

	temp = *head;
	while (temp)
	{
		if ((temp)->fd == fd)
			return (temp);
		temp = (temp)->next;
	}
	new = (t_list_gnl *)malloc(sizeof(t_list_gnl));
	if (new)
	{
		new->fd = fd;
		new->next_line = NULL;
		new->next = *head;
		*head = new;
	}
	return (new);
}

int	gnl_free_all(t_list_gnl **head, t_list_gnl *current, t_line *first_line)
{
	t_line	*temp;

	while (first_line != NULL)
	{
		if (first_line->line)
			free(first_line->line);
		temp = first_line;
		first_line = first_line->next;
		free(temp);
	}
	if (current != NULL)
	{
		if (*head == current)
		{
			*head = (*head)->next;
			free(current);
			return (-1);
		}
		while ((*head)->next != current)
			*head = (*head)->next;
		(*head)->next = current->next;
		free(current);
	}
	return (-1);
}
