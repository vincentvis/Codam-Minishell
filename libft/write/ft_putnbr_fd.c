/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putnbr_fd.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/06 18:14:06 by vvissche      #+#    #+#                 */
/*   Updated: 2021/11/23 18:47:01 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

void	ft_putnbr_fd(int number, int fd)
{
	long	value;
	int		len;
	int		sign;
	char	result[12];

	value = number;
	sign = 0;
	if (number < 0)
	{
		sign = 1;
		value *= -1;
	}
	len = ft_nbrlen_unsigned(value);
	ft_nbrtostr_n(result, value, len, sign);
	write(fd, result, len + sign);
}
