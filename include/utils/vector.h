/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/12 16:55:55 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/05/12 16:56:04 by jowytrzy      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# define VECTOR_DEFAULT_INIT_CAPACITY 32

# include <stddef.h>
# include <stdint.h>

typedef struct s_vect
{
	void	*array;
	size_t	capacity;
	size_t	item_size;
	size_t	number_of_items;
	void	(*del)(void*);
}	t_vect;

void	vector_destroy(t_vect *vector);
void	*vector_push_back(t_vect *target, void *data);
t_vect	*vector_init(size_t initial_capacity, size_t item_size,
			void (*del)(void*));

int8_t	vector_peek_8(t_vect *vector);
int16_t	vector_peek_16(t_vect *vector);
int32_t	vector_peek_32(t_vect *vector);
int64_t	vector_peek_64(t_vect *vector);
void	*vector_peek(t_vect *vector);

#endif
