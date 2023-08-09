/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   she_env_var.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/08 16:38:41 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 18:00:02 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "env/she_env.h"
#include "env/she_env_internal.h"

static t_response	sheenv_env_var_allocs(size_t input_length)
{
	t_env_var	*var;

	var = malloc(sizeof(*var));
	if (var == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	var->data = malloc(sizeof(var->data) * (input_length + 1));
	if (var->data == NULL)
	{
		free(var);
		return (make_response(MALLOC_FAILURE, NULL));
	}
	return (make_response(SUCCESS, var));
}

t_response	sheenv_new_env_var_from_input(const char *input)
{
	const size_t	input_length = ft_strlen(input);
	char			*equals_position;
	t_env_var		*var;
	t_response		response;

	if (sheenv_is_invalid_key(input, IS_PART_OF_INPUT))
		return (make_response(SHEENV_BAD_INPUT, NULL));
	response = sheenv_env_var_allocs(input_length);
	if (response.status != SUCCESS)
		return (response);
	var = response.data;
	ft_memcpy(var->data, input, input_length + 1);
	equals_position = ft_strchr(var->data, '=');
	if (equals_position == NULL)
		var->key_length = input_length;
	else
		var->key_length = equals_position - var->data;
	var->next = NULL;
	return (make_response(SUCCESS, var));
}

t_response	sheenv_new_env_var_from_key(const char *key)
{
	const size_t	key_length = ft_strlen(key);
	t_env_var		*var;
	t_response		response;

	if (sheenv_is_invalid_key(key, IS_NOT_PART_OF_INPUT))
		return (make_response(SHEENV_BAD_INPUT, NULL));
	response = sheenv_env_var_allocs(key_length);
	if (response.status != SUCCESS)
		return (response);
	var = response.data;
	ft_memcpy(var->data, key, key_length + 1);
	var->key_length = key_length;
	var->next = NULL;
	return (make_response(SUCCESS, var));
}

t_response	sheenv_new_env_var_from_key_value(
		const char *key,
		const char *value
	)
{
	const size_t	key_length = ft_strlen(key);
	const size_t	val_length = ft_strlen(value);
	t_env_var		*var;
	t_response		response;

	if (sheenv_is_invalid_key(key, IS_NOT_PART_OF_INPUT))
		return (make_response(SHEENV_BAD_INPUT, NULL));
	response = sheenv_env_var_allocs(key_length + val_length + 1);
	if (response.status != SUCCESS)
		return (response);
	var = response.data;
	ft_memcpy(var->data, key, key_length + 1);
	var->data[key_length] = '=';
	ft_memcpy(&var->data[key_length + 1], value, val_length + 1);
	var->key_length = key_length;
	var->next = NULL;
	return (make_response(SUCCESS, var));
}
