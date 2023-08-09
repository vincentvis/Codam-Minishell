
#pragma once

#include <stdbool.h>
#include "debug/minishell_debug.nonorm"

#define MAKE_COPY 1
#define AS_LITERAL 0

#define TEST_HOME_DIR "/users/testing/username"
#define TEST_PWD "/users/testing/current_dir"
#define TEST_USERNAME "username"
#define TEST_SHELL_NAME "minishell_testing"

t_token	*param_make_token(enum e_token_type type, char *string, bool alloc);
t_token	*make_token(enum e_token_type type, char *string, bool alloc);

typedef struct {
	t_token				*input;
	t_token				*expected_output;
	enum e_status_code	expected_status;
	bool				in_place_modification;
	int					func;
	int					setup;
	int					teardown;
}	t_token_content_expansion_test;

typedef t_response	(*f_func)(t_token *token);
typedef void		(*f_setup_teardown)(void);

void	setup_fake_env(void);
void	teardown_fake_env(void);
void	setup_exitcode(void);
void	teardown_exitcode(void);
