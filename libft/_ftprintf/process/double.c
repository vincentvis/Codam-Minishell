/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   double.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/02/02 13:30:49 by vincent       #+#    #+#                 */
/*   Updated: 2022/04/12 15:49:46 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

int	ipf__process_double(t_pfdata **pfdata, t_spec *fmt)
{
	char	*part;

	part = &(*pfdata)->dest[(*pfdata)->pos];
	ft_memcpy(part + fmt->strlen.start,
		fmt->bigint->str + fmt->bigint->startpos, fmt->strlen.value);
	ft_memset(part + fmt->width.start, fmt->width.str[0], fmt->width.length);
	ft_memcpy(part + fmt->flags.start, fmt->flags.str, fmt->flags.length);
	if (fmt->precision.value >= BIGINT_DECIMAL_MAX - 1)
		ft_memset(part + fmt->precision.start, '0',
			fmt->strlen.length - fmt->strlen.value);
	if ((fmt->flags.value & FLG_ZERO) && (fmt->arg.llu >> 63)
		&& !fmt->bigint->is_nan && !fmt->bigint->is_inf)
	{
		part[fmt->strlen.start] = '0';
		part[0] = '-';
	}
	fmt->bigint = ft_bigint_destroy(fmt->bigint);
	return (fmt->strlen.length);
}
