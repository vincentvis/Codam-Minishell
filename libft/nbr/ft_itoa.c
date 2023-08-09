/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_itoa.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/05 17:10:40 by vvissche      #+#    #+#                 */
/*   Updated: 2021/11/06 17:52:16 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	char	*result;
	long	num;
	int		sign;

	num = n;
	sign = 0;
	if (n < 0)
	{
		num *= -1;
		sign = 1;
	}
	ft_nbrtoastr(&result, num, sign);
	return (result);
}
