/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_wcslen.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/10/29 09:39:21 by vvissche      #+#    #+#                 */
/*   Updated: 2021/03/23 18:28:21 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_wcslen(const int *str)
{
	int	len;

	len = 0;
	while (str[len] != L'\0')
		len++;
	return (len);
}
