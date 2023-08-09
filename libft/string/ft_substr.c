/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_substr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/04 17:47:56 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 14:16:03 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	const size_t	initial_length = ft_strlen(s);
	char			*str;
	size_t			size;

	if (initial_length == 0 || start > initial_length)
		return (ft_memdup("", 2));
	size = len;
	if (start + len > initial_length)
		size = initial_length - start;
	str = malloc(sizeof(*str) * (size + 1));
	if (str == NULL)
		return (NULL);
	ft_memcpy(str, s + start, size);
	str[size] = '\0';
	return (str);
}
