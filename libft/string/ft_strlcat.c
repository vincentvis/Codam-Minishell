/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlcat.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/29 10:06:45 by vvissche      #+#    #+#                 */
/*   Updated: 2021/11/23 18:48:26 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t requested_size)
{
	const size_t	src_length = ft_strlen(src);
	size_t			dst_length;
	size_t			i;

	if (requested_size == 0)
		return (src_length);
	i = 0;
	dst_length = ft_strlen(dst);
	if (requested_size <= dst_length)
		return (src_length + requested_size);
	while (i < (requested_size - dst_length - 1) && src[i])
	{
		dst[dst_length + i] = src[i];
		i++;
	}
	dst[dst_length + i] = '\0';
	return (dst_length + src_length);
}
