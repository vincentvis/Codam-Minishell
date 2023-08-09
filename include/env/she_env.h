/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   she_env.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/16 20:38:31 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 17:35:55 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHE_ENV_H
# define SHE_ENV_H

# include "utils/status.h"
# include <stddef.h>
# include <stdbool.h>

enum e_stupid_norm_not_allowing_this_in_a_preprocessor_define {
	HANDLE_EMPTY_VALUES = true,
	SKIP_EMPTY_VALUES = false
};

typedef struct s_env_var
{
	char				*data;
	size_t				key_length;
	struct s_env_var	*next;
}	t_env_var;

typedef struct s_env_base
{
	t_env_var		*head;
	t_env_var		*tail;
	size_t			count_keys;
	size_t			count_values;
	bool			initialised;
}	t_env_base;

t_env_base	*sheenv(void);
t_response	sheenv_init(char const **env);
void		sheenv_destroy(void);

/*
	CRUD (Create Read Update Delete) + S
	--------------------------------
	"store" is not really part of CRUD but I think it's a useful
	combination of create & update..
	And I've chosen words with the same length for prettyness reasons

	Create = Create  => Create new, assume it does not exist
	Read   = Get     => Get from storage - 'get' because it feels more natural
	Update = Update  => Updates data, assume it exists
	Delete = Unset   => Removes data - named 'unset' because of `unset` buildin
	---
			 Store  => Checks for existence first, then Updates or Creates

	In practice, Get, Store and Unset would be primarily used
	create & update are internal helper functions, and live in a separate header
 */

t_response	sheenv_get_struct_by_key(const char *key);
t_response	sheenv_get_value_by_key(const char *key);
t_response	sheenv_get_struct_by_input(const char *input);
t_response	sheenv_get_value_by_input(const char *input);

t_response	sheenv_unset(const char *key);

t_response	sheenv_store_from_input(const char *input);
t_response	sheenv_store_from_key(const char *key);
t_response	sheenv_store_from_key_value(const char *key, const char *value);

t_response	sheenv_to_array(bool handle_empty);

int			sheenv_print(bool force_print_empty);
int			sheenv_print_export(void);

#endif
