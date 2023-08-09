/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   she_env_create.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/08 16:38:41 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/26 18:15:16 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "env/she_env.h"
#include "env/she_env_internal.h"

static void	create_in_empty_list(t_env_base *const env_base, t_env_var *data)
{
	env_base->head = data;
	env_base->tail = data;
	env_base->tail->next = NULL;
}

t_response	sheenv_create_from_input(const char *input)
{
	t_env_base *const	env_base = sheenv();
	t_response			new;

	if (env_base->initialised == false)
		return (make_response(SHEENV_UNINITIALIZED, NULL));
	if (sheenv_is_invalid_key(input, IS_PART_OF_INPUT))
		return (make_response(SHEENV_BAD_INPUT, NULL));
	new = sheenv_new_env_var_from_input(input);
	if (new.status != SUCCESS)
		return (new);
	env_base->count_keys++;
	if (((t_env_var *)new.data)
		->data[((t_env_var *)new.data)->key_length] == '=')
		env_base->count_values++;
	if (env_base->head == NULL)
	{
		create_in_empty_list(env_base, new.data);
		return (new);
	}
	env_base->tail->next = new.data;
	env_base->tail = env_base->tail->next;
	return (new);
}

t_response	sheenv_create_from_key(const char *key)
{
	t_env_base *const	env_base = sheenv();
	t_response			new;

	if (env_base->initialised == false)
		return (make_response(SHEENV_UNINITIALIZED, NULL));
	if (sheenv_is_invalid_key(key, IS_NOT_PART_OF_INPUT))
		return (make_response(SHEENV_BAD_INPUT, NULL));
	new = sheenv_new_env_var_from_key(key);
	if (new.status != SUCCESS)
		return (new);
	env_base->count_keys++;
	if (env_base->head == NULL)
	{
		create_in_empty_list(env_base, new.data);
		return (new);
	}
	env_base->tail->next = new.data;
	env_base->tail = env_base->tail->next;
	return (new);
}

t_response	sheenv_create_from_key_value(const char *key, const char *value)
{
	t_env_base *const	env_base = sheenv();
	t_response			new;

	if (env_base->initialised == false)
		return (make_response(SHEENV_UNINITIALIZED, NULL));
	if (sheenv_is_invalid_key(key, IS_NOT_PART_OF_INPUT) || value == NULL)
		return (make_response(SHEENV_BAD_INPUT, NULL));
	new = sheenv_new_env_var_from_key_value(key, value);
	if (new.status != SUCCESS)
		return (new);
	env_base->count_keys++;
	env_base->count_values++;
	if (env_base->head == NULL)
	{
		create_in_empty_list(env_base, new.data);
		return (new);
	}
	env_base->tail->next = new.data;
	env_base->tail = env_base->tail->next;
	return (new);
}
