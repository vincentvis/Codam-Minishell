/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strtrim.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/05 08:36:42 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 14:15:54 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	const size_t	set_len = ft_strlen(set);
	size_t			trim_end;
	char			*str;
	char			*trimmed;

	str = (char *)s1;
	if (!set)
		return (ft_strdup(str));
	if (set_len == 0)
		return (ft_strdup(str));
	while (ft_memchr(set, *str, set_len) && *str)
		str++;
	if (*str == '\0')
		return (ft_strdup(""));
	trim_end = ft_strlen(str) - 1;
	while (ft_memchr(set, str[trim_end], set_len))
		trim_end--;
	trim_end++;
	trimmed = malloc(sizeof(*trimmed) * (trim_end + 1));
	if (trimmed == NULL)
		return (NULL);
	ft_memcpy(trimmed, str, trim_end);
	trimmed[trim_end] = '\0';
	return (trimmed);
}
