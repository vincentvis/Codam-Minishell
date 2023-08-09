/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strnstr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/30 14:11:30 by vvissche      #+#    #+#                 */
/*   Updated: 2021/11/23 18:49:29 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t		i;
	size_t		search_limit;
	size_t		len_ndl;

	len_ndl = ft_strlen(needle);
	i = ft_strnlen(haystack, len_ndl);
	if (i < len_ndl || len_ndl > len)
		return (NULL);
	i = 0;
	if (ft_memcmp(haystack, needle, len_ndl) == 0 || needle[0] == '\0')
		return ((char *)haystack);
	search_limit = len - len_ndl;
	while (i <= search_limit && *haystack != '\0')
	{
		if (*haystack == needle[0])
		{
			if (ft_memcmp(haystack, needle, len_ndl) == 0)
				return ((char *)haystack);
		}
		i++;
		haystack++;
	}
	return (NULL);
}
