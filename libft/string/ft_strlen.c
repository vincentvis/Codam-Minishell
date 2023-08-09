/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlen.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/29 09:39:21 by vvissche      #+#    #+#                 */
/*   Updated: 2021/03/23 11:51:29 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	char	*m;

	m = (char *)s;
	while (*m)
	{
		if (!*(m + 1) || !*(m + 2))
			return ((m + 1 + !!*(m + 1)) - s);
		if (!*(m + 3) || !*(m + 4))
			return ((m + 3 + !!*(m + 3)) - s);
		if (!*(m + 5) || !*(m + 6))
			return ((m + 5 + !!*(m + 5)) - s);
		if (!*(m + 7) || !*(m + 8))
			return ((m + 7 + !!*(m + 7)) - s);
		if (!*(m + 9) || !*(m + 10))
			return ((m + 9 + !!*(m + 9)) - s);
		if (!*(m + 11) || !*(m + 12))
			return ((m + 11 + !!*(m + 11)) - s);
		if (!*(m + 13) || !*(m + 14))
			return ((m + 13 + !!*(m + 13)) - s);
		if (!*(m + 15) || !*(m + 16))
			return ((m + 15 + !!*(m + 15)) - s);
		m += 17;
	}
	return (m - s);
}