/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memdup.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/13 10:22:31 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 14:16:50 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
	Basicaly strdup where you know the length of the string..
*/

void	*ft_memdup(const char *s1, size_t n)
{
	char	*dup;

	dup = malloc(sizeof(*dup) * n);
	if (dup == NULL)
		return (NULL);
	ft_memcpy(dup, s1, n);
	return (dup);
}
