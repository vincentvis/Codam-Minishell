/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   she_env_internal.h                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/16 20:38:31 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/18 17:45:30 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHE_ENV_INTERNAL_H
# define SHE_ENV_INTERNAL_H

# include "utils/status.h"
# include <stdbool.h>

enum e_stupid_norm_not_allowing_this_in_a_preprocessor_define_again {
	IS_PART_OF_INPUT = true,
	IS_NOT_PART_OF_INPUT = false
};

t_response	sheenv_new_env_var_from_input(const char *input)
			__attribute__((nonnull));

t_response	sheenv_new_env_var_from_key(const char *key)
			__attribute__((nonnull));

t_response	sheenv_new_env_var_from_key_value(const char *key,
				const char *value)
			__attribute__((nonnull));

bool		sheenv_cmp_string_with_key(const char *data, const char *key,
				size_t key_length)
			__attribute__((always_inline));

bool		sheenv_is_invalid_key(const char *key, bool is_part_of_input)
			__attribute__((pure));

t_response	sheenv_create_from_input(const char *input);
t_response	sheenv_create_from_key(const char *key);
t_response	sheenv_create_from_key_value(const char *key, const char *value);

t_response	sheenv_update_from_input(const char *input);
t_response	sheenv_update_from_key_value(const char *key, const char *value);

int			sheenv_fail_and_free(void *to_free, int return_value);

#endif
