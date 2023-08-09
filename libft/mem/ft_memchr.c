/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memchr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/01 08:36:12 by vvissche      #+#    #+#                 */
/*   Updated: 2021/11/23 18:50:41 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s_in, int c_in, size_t n)
{
	unsigned char	c;
	unsigned char	*s;

	c = (unsigned char)c_in;
	s = (unsigned char *)s_in;
	while (n > 0)
	{
		if (*s == c)
			return ((void *)s);
		s++;
		n--;
	}
	return (NULL);
}
