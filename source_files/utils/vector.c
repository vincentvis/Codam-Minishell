/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/06 09:16:27 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/31 11:14:16 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdbool.h>
#include "utils/vector.h"

static bool	vector_resize(t_vect *vector)
{
	void	*new_array;

	new_array = malloc((vector->capacity * 2) * vector->item_size);
	if (new_array == NULL)
		return (false);
	vector->capacity = vector->capacity * 2;
	ft_memcpy(new_array, vector->array,
		vector->number_of_items * vector->item_size);
	free(vector->array);
	vector->array = new_array;
	return (true);
}

void	vector_destroy(t_vect *vector)
{
	size_t			iterator;
	unsigned char	*pointer;

	if (vector == NULL)
		return ;
	if (vector->del != NULL)
	{
		iterator = 0;
		pointer = vector->array;
		while (iterator < vector->number_of_items)
		{
			vector->del(pointer + (iterator * vector->item_size));
			iterator++;
		}
	}
	free(vector->array);
	free(vector);
}

void	*vector_push_back(t_vect *target, void *data)
{
	unsigned char	*insert_point;

	if (target == NULL)
		return (NULL);
	if (target->capacity == target->number_of_items)
	{
		if (vector_resize(target) == false)
			return (NULL);
	}
	insert_point = ((unsigned char *)target->array)
		+ (target->number_of_items * target->item_size);
	ft_memcpy(insert_point, data, target->item_size);
	target->number_of_items++;
	return (target->array);
}

t_vect	*vector_init(size_t initial_capacity, size_t item_size,
		void (*del)(void*))
{
	t_vect	*vector;

	if (initial_capacity == 0)
		initial_capacity = VECTOR_DEFAULT_INIT_CAPACITY;
	vector = malloc(sizeof(*vector));
	if (vector == NULL)
		return (NULL);
	vector->array = malloc(initial_capacity * item_size);
	if (vector->array == NULL)
	{
		free(vector);
		return (NULL);
	}
	vector->capacity = initial_capacity;
	vector->item_size = item_size;
	vector->del = del;
	vector->number_of_items = 0;
	return (vector);
}
