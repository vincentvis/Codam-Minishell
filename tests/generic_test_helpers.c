
#include "../include/parser/parser.h"
#include "../include/utils/status.h"
#include <criterion/criterion.h>
#include "../include/utils/status.h"
#include "../libft/include/libft.h"
#include "../include/env/she_env.h"
#include "../include/env/she_env_internal.h"

const char *get_token_type_str(int type)
{
	static const char	*type_strings[] = {
		[WORD_TYPE] = "word_type",
		[IO_NUMBER_TYPE] = "io_number_type",
		[PIPE_TYPE] = "pipe_type",
		[LESS_TYPE] = "less_type",
		[DLESS_TYPE] = "dless_type",
		[GREAT_TYPE] = "great_type",
		[DGREAT_TYPE] = "dgreat_type"
	};

	return (type_strings[type]);
}

// from enum_value_to_string file

const char	*debug_token_type_from_enum(int type)
{
	static const char *data[] = {
		[WORD_TYPE] = "word_type",
		[QUOTE_TYPE] = "quote_type",
		[DQUOTE_TYPE] = "dquote_type",
		[HERE_EOF_TYPE] = "here_eof_type",
		[HERE_EOF_EXP_TYPE] = "here_eof_exp_type",
		[VAR_TYPE] = "var_type",
		[EXITCODE_TYPE] = "exitcode_type",
		[TILDE_TYPE] = "tilde_type",
		[IO_NUMBER_TYPE] = "io_number_type",
		[PIPE_TYPE] = "pipe_type",
		[_REDIRECTION] = "_redirection",
		[LESS_TYPE] = "less_type",
		[DLESS_TYPE] = "dless_type",
		[GREAT_TYPE] = "great_type",
		[DGREAT_TYPE] = "dgreat_type",
		[__INTERNAL_TYPE_END] = "__internal_type_end"
	};

	return (data[type]);
}

const char	*debug_state_type_from_enum(int state)
{
	static const char *data[] = {
		[START_STATE] = "START_STATE",
		[IN_QUOTE_STATE] = "IN_QUOTE_STATE",
		[IN_DQUOTE_STATE] = "IN_DQUOTE_STATE",
		[internal_VALID_TOKEN_END_STATE] = "internal_VALID_TOKEN_END_STATE",
		[DIGIT_STATE] = "DIGIT_STATE",
		[WORD_STATE] = "WORD_STATE",
		[DLESS_STATE] = "DLESS_STATE",
		[DGREAT_STATE] = "DGREAT_STATE",
		[EXITCODE_STATE] = "EXITCODE_STATE",
		[PIPE_STATE] = "PIPE_STATE",
		[LESS_STATE] = "LESS_STATE",
		[GREAT_STATE] = "GREAT_STATE",
		[TILDE_STATE] = "TILDE_STATE",
		[VAR_START_STATE] = "VAR_START_STATE",
		[IN_VAR_STATE] = "IN_VAR_STATE",
		[QUOTE_END] = "QUOTE_END",
		[DQUOTE_END] = "DQUOTE_END",
		[INTERNAL_END_STATE] = "INTERNAL_END_STATE",
	};

	return (data[state]);
}

const char	*debug_status_code_from_enum(int status)
{
	static const char	*data[] = {
		[SUCCESS] = "SUCCESS",
		[BAD_INPUT] = "BAD_INPUT",
		[MALLOC_FAILURE] = "MALLOC_FAILURE",
		[SHEENV_BAD_INPUT] = "SHEENV_BAD_INPUT",
		[SHEENV_DOUBLE_INIT] = "SHEENV_DOUBLE_INIT",
		[SHEENV_UNINITIALIZED] = "SHEENV_UNINITIALIZED",
		[SHEENV_KEY_NOT_FOUND] = "SHEENV_KEY_NOT_FOUND",
		[L_BAD_INPUT] = "L_BAD_INPUT",
		[P_BAD_INPUT] = "P_BAD_INPUT",
		[E_FILE_FAILURE] = "E_FILE_FAILURE",
		[E_FD_DUPLICATION_FAILURE] = "E_FD_DUPLICATION_FAILURE",
		[E_PIPE_FAILURE] = "E_PIPE_FAILURE",
		[E_FORK_FAILURE] = "E_FORK_FAILURE",
		[E_COMMAND_NOT_FOUND] = "E_COMMAND_NOT_FOUND",
	};

	return (data[status]);
}
/*
	Add this function so we can unset stuff and empty our env
 */
t_response	sheenv_unset_from_input(const char *input)
{
	t_env_base *const	env_base = sheenv();
	const size_t		input_length = ft_strlen(input);
	size_t				key_length;
	char				*key;
	t_response			unset_response;

	if (env_base->initialised == false)
		return (make_response(SHEENV_UNINITIALIZED, NULL));
	key = ft_strchr(input, '=');
	key_length = input_length;
	if (key != NULL)
		key_length = key - input;
	key = malloc(sizeof(char) * key_length + 1);
	if (key == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	ft_memcpy(key, input, key_length);
	key[key_length] = '\0';
	unset_response = sheenv_unset(key);
	free(key);
	return (unset_response);
}

/*
	Remove defaults to actually start with an empty env..
*/
void	make_sure_env_is_empty(void)
{
	t_env_base	*initial_base = sheenv();
	t_env_var	*tail = initial_base->tail;
	while (tail)
	{
		sheenv_unset_from_input(tail->data);
		tail = initial_base->tail;
	}
}

void	setup_empty_env(void)
{
	const char *empty[] = {NULL};
	t_response	init = sheenv_init(empty);
	cr_assert(init.status == SUCCESS);
	cr_assert_not_null(init.data);

	t_env_base *env_base = init.data;
	while (env_base->tail)
	{
		sheenv_unset_from_input(env_base->tail->data);
	}
	cr_assert_null(env_base->head);
	cr_assert_null(env_base->tail);
}

