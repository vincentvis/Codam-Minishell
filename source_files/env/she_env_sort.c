/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   she_env_sort.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/20 09:48:11 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 17:59:31 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "env/she_env.h"
#include "env/she_env_internal.h"

static void	move_elems_forward(t_env_var **array, size_t min, size_t max)
{
	while (min <= max)
	{
		array[min - 1] = array[min];
		min++;
	}
}

static void	insert_into_array(
		t_env_var **array,
		size_t first_elem_index,
		size_t max,
		t_env_var *elem
	)
{
	size_t	check;

	check = max;
	while (check > first_elem_index)
	{
		if (ft_strncmp(elem->data, array[check]->data, array[check]->key_length)
			> 0)
			break ;
		check--;
	}
	if (check == first_elem_index)
	{
		array[check] = elem;
		return ;
	}
	move_elems_forward(array, first_elem_index, check);
	array[check] = elem;
}

static void	sheenv_order_lexograpically(
		t_env_var **array,
		const t_env_var *env,
		const size_t size
	)
{
	size_t		first_elem_index;
	t_env_var	*current_env;

	first_elem_index = size - 1;
	current_env = env->next;
	array[size] = (t_env_var *)env;
	while (current_env)
	{
		insert_into_array(array, first_elem_index, size, current_env);
		current_env = current_env->next;
		first_elem_index--;
	}
}

static int	print_declare_line_for_export(t_env_var *elem)
{
	const char	*prefix = "declare -x ";
	const int	mid = elem->key_length + 1;

	if (elem->data[elem->key_length] == '=')
	{
		if (ft_printf("%s%.*s\"%s\"\n",
				prefix, mid, elem->data, elem->data + mid) == -1)
			return (-1);
	}
	else
	{
		if (ft_printf("%s%s\n", prefix, elem->data) == -1)
			return (-1);
	}
	return (SUCCESS);
}

int	sheenv_print_export(void)
{
	t_env_base *const	env_base = sheenv();
	t_env_var			*env;
	t_env_var			**sorted;
	t_env_var			**ptr_to_free;

	if (env_base->initialised != true)
		return (SHEENV_UNINITIALIZED);
	if (env_base->head == NULL)
		return (SUCCESS);
	env = env_base->head;
	ptr_to_free = ft_calloc(env_base->count_keys + 1, sizeof(*sorted));
	sorted = ptr_to_free;
	if (sorted == NULL)
		return (MALLOC_FAILURE);
	sheenv_order_lexograpically(sorted, env, env_base->count_keys - 1);
	while (*sorted)
	{
		if (print_declare_line_for_export(*sorted) == -1)
			return (sheenv_fail_and_free(ptr_to_free, 1));
		sorted++;
	}
	free(ptr_to_free);
	return (SUCCESS);
}
