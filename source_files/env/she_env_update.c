/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   she_env_update.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/15 17:06:22 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 16:38:53 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "utils/status.h"
#include "env/she_env.h"
#include "env/she_env_internal.h"

static t_response	sheenv_update(t_response existing_item, t_response new_item)
{
	t_env_var			*existing_var;
	t_env_var			*new_var;

	existing_var = existing_item.data;
	new_var = new_item.data;
	if (new_var->data[new_var->key_length] == '\0')
	{
		free(new_var->data);
		free(new_var);
		return (make_response(SUCCESS, existing_var));
	}
	if (existing_var->data[existing_var->key_length] != '='
		&& new_var->data[new_var->key_length] == '=')
		sheenv()->count_values++;
	free(existing_var->data);
	existing_var->data = new_var->data;
	free(new_var);
	return (make_response(SUCCESS, existing_var));
}

t_response	sheenv_update_from_key_value(const char *key, const char *value)
{
	t_response		existing_item;
	t_response		new_item;

	if (sheenv_is_invalid_key(key, IS_NOT_PART_OF_INPUT) || value == NULL)
		return (make_response(SHEENV_BAD_INPUT, NULL));
	existing_item = sheenv_get_struct_by_key(key);
	if (existing_item.status != SUCCESS)
		return (existing_item);
	new_item = sheenv_new_env_var_from_key_value(key, value);
	if (new_item.status != SUCCESS)
		return (new_item);
	return (sheenv_update(existing_item, new_item));
}

t_response	sheenv_update_from_input(const char *input)
{
	t_response		existing_item;
	t_response		new_item;

	if (sheenv_is_invalid_key(input, IS_PART_OF_INPUT))
		return (make_response(SHEENV_BAD_INPUT, NULL));
	existing_item = sheenv_get_struct_by_input(input);
	if (existing_item.status != SUCCESS)
		return (existing_item);
	new_item = sheenv_new_env_var_from_input(input);
	if (new_item.status != SUCCESS)
		return (new_item);
	return (sheenv_update(existing_item, new_item));
}
