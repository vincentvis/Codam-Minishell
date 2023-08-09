
#include "lexer/token.h"
#include "utils/status.h"

const char	*debug_token_type_from_enum(int type)
{
	static const char *data[] = {
		[WORD_TYPE] = "word_type",
		[QUOTE_TYPE] = "quote_type",
		[DQUOTE_TYPE] = "dquote_type",
		[HERE_EOF_TYPE] = "here_eof_type",
		[HERE_EOF_EXP_TYPE] = "here_eof_exp_type",
		[VAR_TYPE] = "var_type",
		[DOLLARSIGN_TYPE] = "dollarsign_type",
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
