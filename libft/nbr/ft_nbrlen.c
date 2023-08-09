/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_nbrlen.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/12 11:26:54 by vvissche      #+#    #+#                 */
/*   Updated: 2021/03/23 16:55:14 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

uint_fast8_t	ft_nbrlen(long value)
{
	int	sign;

	sign = 0;
	if (value < 0)
		sign = 1;
	if (value == LONG_MIN)
		value++;
	if (value < 0)
		value *= -1;
	return (sign + ft_nbrlen_unsigned(value));
}

uint_fast8_t	ft_nbrlen_base(long value, long base)
{
	int	sign;

	sign = 0;
	if (value < 0)
		sign = 1;
	if (value == LONG_MIN)
		value++;
	if (value < 0)
		value *= -1;
	return (sign + ft_nbrlen_unsigned_base(value, base));
}
