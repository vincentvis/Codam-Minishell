/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   she_env_unset.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/20 09:48:11 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 17:59:46 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "env/she_env.h"
#include "env/she_env_internal.h"

static t_response	sheenv_unset_head(t_env_base *env_base)
{
	t_env_var	*to_unset;

	to_unset = env_base->head;
	env_base->count_keys--;
	if (to_unset->data[to_unset->key_length] == '=')
		env_base->count_values--;
	if (env_base->head == env_base->tail)
		env_base->tail = NULL;
	env_base->head = env_base->head->next;
	free(to_unset->data);
	free(to_unset);
	return (make_response(SUCCESS, NULL));
}

static t_response	sheenv_unset_one(
		t_env_var *prev,
		t_env_var *to_unset,
		t_env_base *env_base
	)
{
	env_base->count_keys--;
	if (to_unset->data[to_unset->key_length] == '=')
		env_base->count_values--;
	if (env_base->tail == to_unset)
		env_base->tail = prev;
	prev->next = to_unset->next;
	free(to_unset->data);
	free(to_unset);
	return (make_response(SUCCESS, NULL));
}

t_response	sheenv_unset(const char *key)
{
	t_env_base *const	env_base = sheenv();
	t_env_var			*current;
	t_env_var			*previous;
	size_t				key_length;

	if (env_base->initialised == false)
		return (make_response(SHEENV_UNINITIALIZED, NULL));
	if (sheenv_is_invalid_key(key, IS_NOT_PART_OF_INPUT))
		return (make_response(SHEENV_BAD_INPUT, NULL));
	key_length = ft_strlen(key);
	current = env_base->head;
	if (env_base->head == NULL)
		return (make_response(SHEENV_KEY_NOT_FOUND, NULL));
	if (sheenv_cmp_string_with_key(current->data, key, key_length))
		return (sheenv_unset_head(env_base));
	previous = current;
	current = current->next;
	while (current)
	{
		if (sheenv_cmp_string_with_key(current->data, key, key_length))
			return (sheenv_unset_one(previous, current, env_base));
		previous = current;
		current = current->next;
	}
	return (make_response(SHEENV_KEY_NOT_FOUND, NULL));
}
