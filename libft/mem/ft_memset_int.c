/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memset_int.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/04 08:03:46 by vvissche      #+#    #+#                 */
/*   Updated: 2021/03/23 12:37:52 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_memset_int_unrolled(void *b, unsigned int c, size_t steps)
{
	unsigned long	longword;

	longword = c;
	longword |= longword << 32;
	while (steps >= 64)
	{
		((unsigned long *)b)[0] = longword;
		((unsigned long *)b)[1] = longword;
		((unsigned long *)b)[2] = longword;
		((unsigned long *)b)[3] = longword;
		((unsigned long *)b)[4] = longword;
		((unsigned long *)b)[5] = longword;
		((unsigned long *)b)[6] = longword;
		((unsigned long *)b)[7] = longword;
		b = ((char *)(b)) + 64;
		steps -= 64;
	}
	return (steps);
}

static void	ft_memset_int_int(void *b, unsigned int c, size_t steps)
{
	while (steps >= 4)
	{
		((unsigned int *)b)[0] = c;
		b = ((char *)b) + 4;
		steps -= 4;
	}
}

/*
** Assume aligned pointer
*/

void	*ft_memset_int(void *b, unsigned int c, size_t len)
{
	size_t		steps;
	void		*ptr;

	steps = len;
	ptr = b;
	if (steps < 8)
		return (b);
	if (steps >= 64)
		steps = ft_memset_int_unrolled(ptr, c, steps);
	if (steps >= 4)
		ft_memset_int_int(ptr + (len - steps), c, steps);
	return (b);
}
