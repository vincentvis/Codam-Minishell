/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nbr.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/16 18:21:53 by vincent       #+#    #+#                 */
/*   Updated: 2022/04/12 15:49:51 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

/*
** Process number spec
**
** Description
** Gets an input spec and processes it for numbers
** Writes the parts (number, width, precision, flag chars) to the correct
** positions, calculated in previous steps
**
** Parameters
** [t_pfdata]		{pfdata}	Pointer of main data object
** [t_spec]			{fmt}		spec of current fmt
*/

int	ipf__process_nbr(t_pfdata **pfdata, t_spec *fmt)
{
	char			*part;

	part = &(*pfdata)->dest[(*pfdata)->pos];
	ft_nbrtostr_n(&part[fmt->strlen.start], fmt->arg.lli, fmt->strlen.value, 0);
	ft_memset(&part[fmt->width.start], fmt->width.str[0], fmt->width.length);
	ft_memset(&part[fmt->precision.start], fmt->precision.str[0],
		fmt->precision.length);
	ft_memcpy(&part[fmt->flags.start], fmt->flags.str, fmt->flags.length);
	return (fmt->strlen.length);
}

/*
** Process number in some bases
**
** Description
** Gets an input spec and processes it for base numbers
** Writes parts (number, width, precision, flag chars) to the correct
** positions, calculated in previous steps
**
** Parameters
** [t_pfdata]		{pfdata}	Pointer of main data object
** [t_spec]			{fmt}		spec of current fmt
*/

int	ipf__process_nbr_base(t_pfdata **pfdata, t_spec *fmt)
{
	char	*part;

	part = &(*pfdata)->dest[(*pfdata)->pos];
	if (fmt->type == 'x' || fmt->type == 'X')
		ft_nbrtostr_n_base16(&part[fmt->strlen.start], fmt->arg.llu,
			fmt->strlen.value, fmt->type);
	if (fmt->type == 'o')
		ft_nbrtostr_base(&part[fmt->strlen.start], fmt->arg.llu, 8);
	if (fmt->type == 'u')
		ft_nbrtostr_n(&part[fmt->strlen.start], fmt->arg.llu,
			fmt->strlen.value, 0);
	ft_memset(&part[fmt->width.start], fmt->width.str[0], fmt->width.length);
	ft_memset(&part[fmt->precision.start], fmt->precision.str[0],
		fmt->precision.length);
	ft_memcpy(&part[fmt->flags.start], fmt->flags.str, fmt->flags.length);
	return (fmt->strlen.length);
}
