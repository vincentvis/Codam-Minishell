/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   string.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/16 18:05:54 by vincent       #+#    #+#                 */
/*   Updated: 2022/04/12 15:50:02 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

/*
** Function process char
**
** Description
** Puts a character in the correct spot
**
** Parameters
** [t_pfdata]		{pfdata}	Pointer of main data object
** [t_spec]			{fmt}		Current FMT object
**
** Return Value
** [int]						Always one, i guess
*/

int	ipf__process_char(t_pfdata **pfdata, t_spec *fmt)
{
	char	*part;

	part = &(*pfdata)->dest[(*pfdata)->pos];
	if (fmt->length == LNG_LONG)
		ipf__utf8(&part[fmt->strlen.start], fmt->arg.lli, fmt->strlen.value);
	else
		ft_memcpy(&part[fmt->strlen.start], &fmt->arg.lli, fmt->strlen.value);
	ft_memset(&part[fmt->width.start], fmt->width.str[0], fmt->width.length);
	ft_memcpy(&part[fmt->flags.start], fmt->flags.str, fmt->flags.length);
	return (fmt->strlen.length);
}

/*
** Function put string
**
** Description
** Puts a string in the correct spot
** Can print types 's' (normal) and type 't' which is my special type to get
** the string chars from the formatstring (to write non fmt parts)
**
** Parameters
** [t_pfdata]		{pfdata}	Pointer of main data object
** [t_spec]			{fmt}		Current FMT object
**
** Return Value
** [int]						strlen
*/

int	ipf__process_string(t_pfdata **pfdata, t_spec *fmt)
{
	char	*part;
	int		start;

	start = fmt->strlen.start;
	if (fmt->type == 't')
		start = 0;
	part = &(*pfdata)->dest[(*pfdata)->pos];
	if (fmt->length == LNG_LONG)
		ipf__utf8_str(&part[start], fmt->arg.ls, fmt->strlen.value);
	else
		ft_memcpy(&part[start], fmt->arg.s, fmt->strlen.value);
	ft_memset(&part[fmt->width.start], fmt->width.str[0], fmt->width.length);
	ft_memcpy(&part[fmt->flags.start], fmt->flags.str, fmt->flags.length);
	return (fmt->strlen.length);
}
