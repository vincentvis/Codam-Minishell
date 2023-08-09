/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memset.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/04 08:03:46 by vvissche      #+#    #+#                 */
/*   Updated: 2021/03/23 12:30:23 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_memset_longu(void *b, int c, size_t steps)
{
	long	longword;

	longword = (unsigned char)c;
	longword |= longword << 8;
	longword |= longword << 16;
	longword |= (longword << 16) << 16;
	while (steps)
	{
		((unsigned long int *)b)[0] = longword;
		((unsigned long int *)b)[1] = longword;
		((unsigned long int *)b)[2] = longword;
		((unsigned long int *)b)[3] = longword;
		((unsigned long int *)b)[4] = longword;
		((unsigned long int *)b)[5] = longword;
		((unsigned long int *)b)[6] = longword;
		((unsigned long int *)b)[7] = longword;
		b = (char *)b + 64;
		steps--;
	}
}

static void	ft_memset_long(void *b, int c, size_t steps)
{
	long	longword;

	longword = (unsigned char)c;
	longword |= longword << 8;
	longword |= longword << 16;
	longword |= (longword << 16) << 16;
	while (steps)
	{
		((unsigned long int *)b)[0] = longword;
		b = (char *)b + 8;
		steps--;
	}
}

static void	ft_memset_byte(void *b, unsigned char c, size_t len)
{
	while (len)
	{
		len--;
		((unsigned char *)b)[len] = c;
	}
}

static void	ft_memset_align(void *b, unsigned char c, size_t len)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)b;
	while (len)
	{
		*ptr = c;
		ptr++;
		len--;
	}
}

void	*ft_memset(void *b, int c, size_t len)
{
	size_t		steps;
	size_t		longsteps;
	size_t		algnsteps;

	steps = 0;
	longsteps = 0;
	algnsteps = 0;
	if (len >= 24)
	{
		algnsteps = (-((size_t)((char *)b)) & (sizeof(long) - 1));
		len -= algnsteps;
		ft_memset_align(b, c, algnsteps);
		longsteps = len / 64;
		ft_memset_longu(&((char *)b)[algnsteps], c, longsteps);
		longsteps = longsteps * 64;
		len -= longsteps;
		steps = len / 8;
		ft_memset_long(&((char *)b)[algnsteps + longsteps], c, steps);
		steps = steps * 8;
		len -= steps;
	}
	ft_memset_byte(&((char *)b)[algnsteps + longsteps + steps], \
		(unsigned char)c, len);
	return (b);
}
