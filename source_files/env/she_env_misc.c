/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   she_env_misc.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/20 09:48:11 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/26 18:16:21 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "env/she_env.h"

int	sheenv_print(bool force_print_empty)
{
	t_env_base *const	env_base = sheenv();
	t_env_var			*var;

	if (env_base->initialised == false)
		return (SHEENV_UNINITIALIZED);
	var = env_base->head;
	while (var)
	{
		if (var->data[var->key_length] == '=' || force_print_empty)
		{
			if (ft_printf("%s\n", var->data) == -1)
				return (1);
		}
		var = var->next;
	}
	return (SUCCESS);
}

static t_env_var	*maybe_skip_empty(t_env_var *var, bool handle_empty)
{
	if (handle_empty == HANDLE_EMPTY_VALUES)
		return (var);
	while (var && var->data[var->key_length] == '\0')
		var = var->next;
	return (var);
}

t_response	sheenv_to_array(bool handle_empty)
{
	t_env_base *const	env_base = sheenv();
	t_env_var			*var;
	char				**array;
	size_t				count;

	if (env_base->initialised == false)
		return (make_response(SHEENV_UNINITIALIZED, NULL));
	var = maybe_skip_empty(env_base->head, handle_empty);
	if (var == NULL)
		return (make_response(SUCCESS, NULL));
	array = malloc(sizeof(*array) * env_base->count_keys + 1);
	if (array == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	count = 0;
	while (var)
	{
		array[count] = var->data;
		count++;
		var = maybe_skip_empty(var->next, handle_empty);
	}
	array[count] = NULL;
	return (make_response(SUCCESS, array));
}
