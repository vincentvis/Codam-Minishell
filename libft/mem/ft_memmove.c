/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memmove.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/01 08:36:12 by vvissche      #+#    #+#                 */
/*   Updated: 2021/11/23 18:51:00 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	unsigned char	*dstt;
	unsigned char	*srcc;

	if (dst == src)
		return (dst);
	dstt = (unsigned char *)dst;
	srcc = (unsigned char *)src;
	if (srcc < dstt)
	{
		while (n > 0)
		{
			n--;
			*(dstt + n) = *(srcc + n);
		}
	}
	else
		ft_memcpy(dst, src, n);
	return (dst);
}
