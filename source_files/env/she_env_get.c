/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   she_env_get.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/08 16:38:41 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/26 18:15:21 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "env/she_env.h"
#include "env/she_env_internal.h"

t_response	sheenv_get_struct_by_key(const char *key)
{
	t_env_base *const	env_base = sheenv();
	size_t				key_len;
	t_env_var			*env_var;

	if (env_base->initialised == false)
		return (make_response(SHEENV_UNINITIALIZED, NULL));
	if (sheenv_is_invalid_key(key, IS_NOT_PART_OF_INPUT))
		return (make_response(SHEENV_BAD_INPUT, NULL));
	key_len = ft_strlen(key);
	env_var = env_base->head;
	while (env_var)
	{
		if (sheenv_cmp_string_with_key(env_var->data, key, key_len))
		{
			return (make_response(SUCCESS, env_var));
		}
		env_var = env_var->next;
	}
	return (make_response(SHEENV_KEY_NOT_FOUND, NULL));
}

t_response	sheenv_get_value_by_key(const char *key)
{
	const t_response	get_struct = sheenv_get_struct_by_key(key);
	t_env_var			*env_var;

	if (get_struct.status != SUCCESS || get_struct.data == NULL)
		return (get_struct);
	env_var = get_struct.data;
	if (env_var->data[env_var->key_length] == '\0')
		return (make_response(SUCCESS, env_var->data + env_var->key_length));
	return (make_response(SUCCESS, env_var->data + env_var->key_length + 1));
}

t_response	sheenv_get_struct_by_input(const char *input)
{
	char		*equals_position;
	t_response	get_struct;
	char		*key;

	if (sheenv_is_invalid_key(input, IS_PART_OF_INPUT))
		return (make_response(SHEENV_BAD_INPUT, NULL));
	equals_position = ft_strchr(input, '=');
	if (equals_position == NULL)
		return (sheenv_get_struct_by_key(input));
	key = ft_substr(input, 0, equals_position - input);
	if (key == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	get_struct = sheenv_get_struct_by_key(key);
	free(key);
	return (get_struct);
}

t_response	sheenv_get_value_by_input(const char *input)
{
	const t_response	get_struct = sheenv_get_struct_by_input(input);
	t_env_var			*env_var;

	if (get_struct.status != SUCCESS || get_struct.data == NULL)
		return (get_struct);
	env_var = get_struct.data;
	if (env_var->data[env_var->key_length] == '\0')
		return (make_response(SUCCESS, env_var->data + env_var->key_length));
	return (make_response(SUCCESS, env_var->data + env_var->key_length + 1));
}
