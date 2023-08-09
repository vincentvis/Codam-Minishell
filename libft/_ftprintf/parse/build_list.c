/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   build_list.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/04 17:55:10 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 15:49:14 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

/*
** Function Create Spec (static)
**
** Description
** Allocates t_spec and sets default values
**
** Parameters
** [t_spec **]	{spec}	pointer to where to put the spec
**
** Return value
** [int]		{string}	1 on success, -1 on fail
*/

static int	ipf__new_spec(t_spec **spec)
{
	*spec = ft_calloc(1, sizeof(**spec));
	if (*spec == NULL)
		return (-1);
	(*spec)->flags.str[0] = '0';
	(*spec)->flags.str[1] = 'x';
	(*spec)->width.str[0] = ' ';
	(*spec)->next = NULL;
	return (1);
}

/*
** Function add text spec (static)
**
** Description
** Pushes to end of list a new spec with type 't'
** Of this list item `precision` is used to set startpos in fmt
** `width` is used for strlen
**
** Parameters
** [t_pfdata]	{pfdata}	Pointer of main data object
** [site_t]		{start}		start position in format string
**
** Return value
** [int]		{string}	1 on success, -1 on fail
*/

static int	ipf__add_text_spec(t_pfdata **pfdata, size_t start)
{
	t_spec	*new;

	if (ipf__new_spec(&new) == -1)
	{
		(*pfdata)->error = -1;
		return ((*pfdata)->error);
	}
	new->type = 't';
	new->strlen.start = start;
	if ((*pfdata)->head == NULL)
		(*pfdata)->head = new;
	else
		(*pfdata)->tail->next = new;
	(*pfdata)->tail = new;
	return (1);
}

/*
** Function Set text length (static)
**
** Description
** Set text length of just pieces of text of the input *fmt.
** Nothing has to be done with this, just printed afterwards
**
** Parameters
** [t_pfdata]	{pfdata}	Pointer of main data object
** [site_t]		{start}		current position in format string
**
** Return
** void
*/

static void	ipf__set_text_length(t_pfdata **pfdata, size_t index)
{
	t_spec	*tail;

	tail = (*pfdata)->tail;
	tail->strlen.value = index - tail->strlen.start;
	tail->strlen.length = tail->strlen.value;
	tail->arg.s = (char *)&(*pfdata)->fmt[tail->strlen.start];
	(*pfdata)->t_len += tail->strlen.value;
}

/*
** Function Process Format (static)
**
** Description
** When we arrive in this function the character we are at in our format string
** should be == '%'. So we can put the index one step forward to do actual
** format string parsing.
** We create a new t_spec at the tail of the list for our format string and we
** start calling ipf__parse_* functions
**
** Parameters
** [t_pfdata]	{pfdata}	Pointer of main data object
** [site_t]		{start}		current position in format string
**
** Return value
** [int]		{string}	1 on success, -1 on fail
*/

static int	ipf__process_format(t_pfdata **pfdata, size_t *index)
{
	t_spec	*new;

	*index += 1;
	if (ipf__new_spec(&new) == -1)
	{
		(*pfdata)->error = -1;
		return ((*pfdata)->error);
	}
	(*pfdata)->tail->next = new;
	(*pfdata)->tail = new;
	*index = ipf__parse_flags(pfdata, *index);
	*index = ipf__parse_width(pfdata, *index);
	*index = ipf__parse_precision(pfdata, *index);
	*index = ipf__parse_length(pfdata, *index);
	*index = ipf__parse_type(pfdata, *index);
	return ((*pfdata)->error);
}

/*
** Function Build List
**
** Description
** Main list building function for setting up a complete linked list
** with t_spec objects containing either type 't' for plain formatstring parts
** or t_spec objects with other type to contain variables from va_list
**
** Goes through whole string in these steps:
** 1. skip all chars until begin of format (%) is reached or EOL
** 2. save length of text before format in our current t_spec (if needed)
** 3. if we reached EOL, break out of loop, we are done
** 4. if we get here, we are on a format starting with %, process it
** 5. if next thing is not new format and not EOL, make new text t_spec
**
** Parameters
** [t_pfdata]	{pfdata}	Pointer of main data object
**
** Return value
** [int]		{string}	1 on success, -1 on fail
*/

int	ipf__build_list(t_pfdata **pfdata)
{
	size_t	index;

	index = 0;
	if (ipf__add_text_spec(pfdata, index) == -1)
	{
		(*pfdata)->error = -1;
		return ((*pfdata)->error);
	}
	while ((*pfdata)->fmt[index] != '\0' && (*pfdata)->error != -1)
	{
		while ((*pfdata)->fmt[index] != '%' && (*pfdata)->fmt[index] != '\0')
			index++;
		if ((*pfdata)->tail->type == 't')
			ipf__set_text_length(pfdata, index);
		if ((*pfdata)->fmt[index] == '\0')
			break ;
		if (ipf__process_format(pfdata, &index) < 0)
			return ((*pfdata)->error);
		if ((*pfdata)->fmt[index] != '%' && (*pfdata)->fmt[index] != '\0')
			if (ipf__add_text_spec(pfdata, index) == -1)
				return (-1);
	}
	return ((*pfdata)->error);
}
