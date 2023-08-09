/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strdup.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/04 16:00:59 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 14:15:29 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	const size_t	length = ft_strlen(s1) + 1;
	char			*copy;

	copy = malloc(sizeof(*copy) * length);
	if (copy == NULL)
		return (NULL);
	ft_memcpy(copy, s1, length);
	return (copy);
}
