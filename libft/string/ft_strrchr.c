/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strrchr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/29 10:06:45 by vvissche      #+#    #+#                 */
/*   Updated: 2021/11/23 18:49:33 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*match;
	char		ch;
	char		*str;

	match = NULL;
	str = (char *)s;
	ch = (char)c;
	if (ch == '\0')
		return ((char *)(str + ft_strlen(s)));
	while (*str != '\0')
	{
		if (*str == ch)
		{
			match = (const char *)str;
		}
		str++;
	}
	return ((char *)match);
}
