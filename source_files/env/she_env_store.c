/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   she_env_store.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/06 09:16:27 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/26 18:15:31 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "env/she_env.h"
#include "env/she_env_internal.h"

t_response	sheenv_store_from_input(const char *input)
{
	t_response	response;

	response = sheenv_update_from_input(input);
	if (response.status == SHEENV_KEY_NOT_FOUND)
		response = sheenv_create_from_input(input);
	return (response);
}

t_response	sheenv_store_from_key(const char *key)
{
	t_response	response;

	response = sheenv_get_struct_by_key(key);
	if (response.status == SHEENV_KEY_NOT_FOUND)
		response = sheenv_create_from_key(key);
	return (response);
}

t_response	sheenv_store_from_key_value(const char *key, const char *value)
{
	t_response	response;

	response = sheenv_update_from_key_value(key, value);
	if (response.status == SHEENV_KEY_NOT_FOUND)
		response = sheenv_create_from_key_value(key, value);
	return (response);
}
