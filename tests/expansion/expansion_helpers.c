
#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lexer/token.h"
#include "utils/exitcode.h"

#include "env/she_env.h"

#include "expansion_helpers.h"
#include "debug/minishell_debug.nonorm"
#include "debug/fail_alloc.hh"

t_token	*param_make_token(t_token_type type, char *string, bool alloc)
{
	t_token	*token = cr_calloc(1, sizeof(*token));
	const size_t length = string ? strlen(string) : 0;

	token->type = type;
	token->content.expanded_word = cr_malloc(sizeof(*token->content.expanded_word) * (length + 1));
	memcpy(token->content.expanded_word, string, length);
	token->content.expanded_word[length] = 0;
	if (alloc)
		token->allocated = true;
	// printf("[tken] %p - making token %i with content: (%p)%s\n", (void*)token, type, (void*)token->content.raw_word, token->content.raw_word);
	token->length = length;
	return (token);
}

t_token	*make_token(t_token_type type, char *string, bool alloc)
{
	t_token	*token = calloc(1, sizeof(*token));
	const size_t length = string ? strlen(string) : 0;

	token->type = type;
	token->content.expanded_word = malloc(sizeof(*token->content.expanded_word) * (length + 1));
	memcpy(token->content.expanded_word, string, length);
	token->content.expanded_word[length] = 0;
	if (alloc)
		token->allocated = true;
	// printf("[tken] %p - making token %i with content: (%p)%s\n", (void*)token, type, (void*)token->content.raw_word, token->content.raw_word);
	token->length = length;
	return (token);
}

void	setup_fake_env(void)
{
	const char *fake_env[] = {
		[0] = "HOME=" TEST_HOME_DIR,
		[1] = "PWD=" TEST_PWD,
		[2] = "USER=" TEST_USERNAME,
		[3] = "SHELL=" TEST_SHELL_NAME,
		[4] = "DLR=DLR $",
		[5] = NULL
	};

	sheenv_init(fake_env);
}

void	teardown_fake_env(void)
{
	sheenv_destroy();
}

void	setup_exitcode(void)
{
	store_exit_code(10);
}

void	teardown_exitcode(void)
{
	store_exit_code(0);
}
