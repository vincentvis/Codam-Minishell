/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fmt_type_n.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/22 19:43:11 by vincent       #+#    #+#                 */
/*   Updated: 2022/04/12 15:49:24 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

/*
** Function to handle type n
**
** Description
** Gets the pointer argument from va_arg
** Sets the amount of chars that will be printed at this point as the value
**
** Parameters
** [t_pfdata]		{pfdata}	Pointer of main data object
** [t_spec]			{fmt}		spec of current fmt
*/

int	ipf__fmt_type_n(t_pfdata **pfdata, t_spec *fmt)
{
	fmt->arg.p = va_arg((*pfdata)->ap, long long *);
	if (fmt->length == LNG_SHRT)
		*((short *)fmt->arg.p) = (short)(*pfdata)->t_len;
	else if (fmt->length == LNG_LONG)
		*((long *)fmt->arg.p) = (long)(*pfdata)->t_len;
	else if (fmt->length == LNG_LLNG)
		*((long long *)fmt->arg.p) = (long long)(*pfdata)->t_len;
	else if (fmt->length == LNG_CHAR)
		*((char *)fmt->arg.p) = (char)(*pfdata)->t_len;
	else
		*((int *)fmt->arg.p) = (int)(*pfdata)->t_len;
	return (0);
}

/*
** Process N function
** Just here because in my jump table some function needs to be called
**
** Parameters
** [t_pfdata]		{pfdata}	Pointer of main data object
** [t_spec]			{fmt}		spec of current fmt
*/

int	ipf__process_n(t_pfdata **pfdata, t_spec *fmt)
{
	(void)pfdata;
	(void)fmt;
	return (0);
}
