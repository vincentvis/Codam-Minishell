/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memcpy.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/01 08:36:12 by vvissche      #+#    #+#                 */
/*   Updated: 2021/11/23 18:50:52 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_memcpy_longu(unsigned char **dst, unsigned char **src, size_t n)
{
	unsigned long	*longdst;
	unsigned long	*longsrc;

	while (n)
	{
		longdst = (unsigned long *)*dst;
		longsrc = (unsigned long *)*src;
		*longdst = *longsrc;
		*(longdst + 1) = *(longsrc + 1);
		*(longdst + 2) = *(longsrc + 2);
		*(longdst + 3) = *(longsrc + 3);
		*(longdst + 4) = *(longsrc + 4);
		*(longdst + 5) = *(longsrc + 5);
		*(longdst + 6) = *(longsrc + 6);
		*(longdst + 7) = *(longsrc + 7);
		(*dst) += 64;
		(*src) += 64;
		n--;
	}
}

static void	ft_memcpy_long(unsigned char **dst, unsigned char **src, size_t n)
{
	unsigned long	*longdst;
	unsigned long	*longsrc;

	while (n)
	{
		longdst = (unsigned long *)*dst;
		longsrc = (unsigned long *)*src;
		*longdst = *longsrc;
		(*dst) += 8;
		(*src) += 8;
		n--;
	}
}

static void	ft_memcpy_byte(unsigned char **dst, unsigned char **src, size_t n)
{
	while (n)
	{
		**dst = **src;
		(*dst) += 1;
		(*src) += 1;
		n--;
	}
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*dstt;
	unsigned char	*srcc;
	size_t			steps;

	if ((!dst && !src) || n == 0)
		return (dst);
	dstt = (unsigned char *)dst;
	srcc = (unsigned char *)src;
	if (n >= 24)
	{
		steps = (-((size_t)(dst)) & (sizeof(long) - 1));
		n -= steps;
		ft_memcpy_byte(&dstt, &srcc, steps);
		steps = n / 64;
		n -= steps * 64;
		ft_memcpy_longu(&dstt, &srcc, steps);
		steps = n / 8;
		n -= steps * 8;
		ft_memcpy_long(&dstt, &srcc, steps);
	}
	ft_memcpy_byte(&dstt, &srcc, n);
	return (dst);
}
