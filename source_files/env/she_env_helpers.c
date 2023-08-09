/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   she_env_helpers.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/20 09:48:11 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 17:57:46 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "libft.h"
#include "env/she_env_internal.h"

bool	sheenv_cmp_string_with_key(
		const char *data,
		const char *key,
		size_t key_length
	)
{
	return (ft_memcmp(data, key, key_length) == 0
		&& (data[key_length] == '=' || data[key_length] == '\0'));
}

bool	sheenv_is_invalid_key(const char *input, bool is_part_of_input)
{
	size_t	i;

	if (input == NULL || *input == '\0'
		|| *input == '=' || ft_isdigit(*input))
		return (true);
	i = 0;
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		i++;
	if (is_part_of_input == IS_PART_OF_INPUT)
		return (input[i] != '=' && input[i] != '\0');
	else
		return (input[i] != '\0');
}

int	sheenv_fail_and_free(void *to_free, int return_value)
{
	free(to_free);
	return (return_value);
}
