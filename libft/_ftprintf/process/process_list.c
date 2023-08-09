/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   process_list.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/27 10:24:27 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 15:54:49 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

/*
** Process List function
**
** Description
** Function loops through all list items and performs the assigned function
** on the list item.
** Action parameter is used to switch between printing or putting it in
** allocated string. 0 = print to fd, 1 is to string (pointer in pfdata)
**
** Parameters
** [t_pfdata]		{pfdata}		Pointer of main data object
** [char**]			{dest}			Destination of writing
** [int]			{needs_malloc}	For if we need to malloc or not
**
** Return value
** [size_t]						amount of characters printed
*/

size_t	ipf__process_list(t_pfdata **pfdata, char **dest, int needs_malloc)
{
	const t_jmp	*jmp = ipf_jump_process();
	t_spec		*curr;
	t_spec		*prev;

	if ((*pfdata)->error == -2)
		ipf__handle_parse_error(pfdata);
	if ((*pfdata)->error == -1)
		return (-1);
	if (needs_malloc)
	{
		*dest = malloc(sizeof(**dest) * ((*pfdata)->t_len + 1));
		if (*dest == NULL)
			return (-1);
	}
	(*dest)[(*pfdata)->t_len] = 0;
	(*pfdata)->dest = *dest;
	curr = (*pfdata)->head;
	while (curr)
	{
		(*pfdata)->pos += jmp[curr->type - JMP_OFFST](pfdata, curr);
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	return ((*pfdata)->pos);
}

/*
** Destroy List function
**
** Description
** Destroys all malloced things in pfdata (the t_list children)
** and then the pfdata itself. Use when something errors
**
** Parameters
** [t_pfdata]		{pfdata}	Pointer of main data object
**
** Return value
** [int]						-1 to indicate error
*/

int	ipf__destroy_list(t_pfdata **pfdata)
{
	t_spec	*destroy;
	t_spec	*current;

	current = (*pfdata)->head;
	while (current)
	{
		destroy = current;
		current = current->next;
		free(destroy);
	}
	free(*pfdata);
	return (-1);
}

int	ipf__handle_parse_error(t_pfdata **pfdata)
{
	t_spec	*check;

	(*pfdata)->error = -1;
	check = (*pfdata)->head;
	while (check)
	{
		if (check->type != 't' && check->type != 'n')
			(*pfdata)->error = 2;
		if (check->next != NULL)
		{
			if (check->next->next == NULL)
			{
				free(check->next);
				check->next = NULL;
				(*pfdata)->tail = check;
				break ;
			}
		}
		check = check->next;
	}
	return ((*pfdata)->error);
}
