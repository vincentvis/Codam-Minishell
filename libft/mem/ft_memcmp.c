/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memcmp.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/01 08:36:12 by vvissche      #+#    #+#                 */
/*   Updated: 2021/11/23 18:50:46 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	a;
	unsigned char	b;
	char			*cs1;
	char			*cs2;

	cs1 = (char *)s1;
	cs2 = (char *)s2;
	while (n != 0)
	{
		a = ((unsigned char *)cs1)[0];
		b = ((unsigned char *)cs2)[0];
		if (a - b != 0)
			return (a - b);
		n -= 1;
		cs1++;
		cs2++;
	}
	return (0);
}
