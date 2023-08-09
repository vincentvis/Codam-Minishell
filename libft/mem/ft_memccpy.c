/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memccpy.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/01 08:36:12 by vvissche      #+#    #+#                 */
/*   Updated: 2021/11/23 18:50:37 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	unsigned char	*dstt;
	unsigned char	*srcc;
	unsigned char	ch;

	if (!dest && !src)
		return (NULL);
	ch = (unsigned char)c;
	dstt = (unsigned char *)dest;
	srcc = (unsigned char *)src;
	while (n > 0)
	{
		*dstt = *srcc;
		if (*srcc == ch)
			return ((void *)(dstt + 1));
		dstt++;
		srcc++;
		n--;
	}
	return (NULL);
}
