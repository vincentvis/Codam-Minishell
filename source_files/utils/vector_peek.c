/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_peek.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/08 16:38:41 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/26 18:17:41 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "utils/vector.h"

int8_t	vector_peek_8(t_vect *vector)
{
	const int8_t	*data = vector->array;

	if (vector->number_of_items == 0)
		return (0);
	return (data[vector->number_of_items - 1]);
}

int16_t	vector_peek_16(t_vect *vector)
{
	const int16_t	*data = vector->array;

	if (vector->number_of_items == 0)
		return (0);
	return (data[vector->number_of_items - 1]);
}

int32_t	vector_peek_32(t_vect *vector)
{
	const int32_t	*data = vector->array;

	if (vector->number_of_items == 0)
		return (0);
	return (data[vector->number_of_items - 1]);
}

int64_t	vector_peek_64(t_vect *vector)
{
	const int64_t	*data = vector->array;

	if (vector->number_of_items == 0)
		return (0);
	return (data[vector->number_of_items - 1]);
}

void	*vector_peek(t_vect	*vector)
{
	unsigned char	*point;

	if (vector->number_of_items == 0)
		return (NULL);
	point = ((unsigned char *)vector->array)
		+ ((vector->number_of_items - 1) * vector->item_size);
	return (point);
}
