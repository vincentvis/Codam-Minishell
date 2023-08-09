/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_wcsnlen.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/30 09:11:17 by vvissche      #+#    #+#                 */
/*   Updated: 2021/03/23 18:28:25 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_wcsnlen(const int *str, size_t maxlen)
{
	size_t	len;

	len = 0;
	while (str[len] != L'\0' && len < maxlen)
		len++;
	return (len);
}
