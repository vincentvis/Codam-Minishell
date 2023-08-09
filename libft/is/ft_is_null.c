/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_is_null.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/30 12:45:02 by vvissche      #+#    #+#                 */
/*   Updated: 2022/01/26 14:02:11 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>

int	ft_is_null(const void *ptr)
{
	if (ptr == NULL)
		return (true);
	return (false);
}
