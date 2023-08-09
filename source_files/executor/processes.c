/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   processes.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/06 15:17:37 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/07 18:09:57 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include "utils/vector.h"
#include "executor/exec_internal.h"

t_vect	*p_id_singleton(enum e_smingleton_actions action)
{
	static t_vect	*p_ids;

	if (p_ids == NULL)
		p_ids = vector_init(16, sizeof(pid_t), NULL);
	if (action == CLEAN)
	{
		vector_destroy(p_ids);
		p_ids = NULL;
	}
	return (p_ids);
}

t_vect	*save_pid(pid_t p_id)
{
	t_vect *const	p_ids = p_id_singleton(GET_ARRAY);

	return (vector_push_back(p_ids, &p_id));
}

pid_t	get_last_pid(void)
{
	t_vect *const	p_ids_vector = p_id_singleton(GET_ARRAY);

	return (vector_peek_32(p_ids_vector));
}

void	clean_pids(void)
{
	p_id_singleton(CLEAN);
}
