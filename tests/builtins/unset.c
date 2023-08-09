#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "builtins/builtins.h"
#include <limits.h>

#include <stdio.h>
#include "env/she_env.h"
#include "../generic_test_helpers.h"
#include "builtins/builtins_internal.h"
#include "utils/readline.h"
#include "utils/minishell_colors.h"

#include <unistd.h>

Test(builtin_unset,
	unset_without_args,
	.init = setup_empty_env)
{
	char *args[] = {
		"unset",
		NULL
	};

	int return_value = builtin_unset(args);
	cr_assert(return_value == BUILTIN_SUCCESS);
}

Test(builtin_unset,
	unset_the_only_arg,
	.init = setup_empty_env)
{
	sheenv_store_from_key_value("TEST", "value");
	char *args[] = {
		"unset",
		"TEST",
		NULL
	};

	int return_value = builtin_unset(args);
	cr_assert(return_value == BUILTIN_SUCCESS);
	cr_assert(sheenv_get_value_by_key("TEST").status == SHEENV_KEY_NOT_FOUND);
	cr_assert_null(sheenv()->head);
	cr_assert_null(sheenv()->tail);
}

Test(builtin_unset,
	unset_non_existent_key,
	.init = setup_empty_env)
{
	sheenv_store_from_key_value("TEST", "value");
	char *args[] = {
		"unset",
		"BLATEST",
		NULL
	};

	int return_value = builtin_unset(args);
	cr_assert(return_value == BUILTIN_SUCCESS);
}

Test(builtin_unset,
	unset_invalid_key,
	.init = setup_empty_env)
{
	sheenv_store_from_key_value("TEST", "value");
	char *args[] = {
		"unset",
		"1BLATEST",
		NULL
	};

	cr_redirect_stderr();

	int return_value = builtin_unset(args);
	cr_assert(return_value == BUILTIN_GENERIC_FAILURE);

	char expected_stderr[1024];
	sprintf(expected_stderr,
		"%s: unset: `%s`: not a valid identifier\n",
		ERROR_EMOJI, args[1]);

	cr_assert_stderr_eq_str(expected_stderr, "");
}

Test(builtin_unset,
	unset_invalid_key_because_equals,
	.init = setup_empty_env)
{
	sheenv_store_from_key_value("TEST", "value");
	char *args[] = {
		"unset",
		"TEST=",
		NULL
	};

	cr_redirect_stderr();

	int return_value = builtin_unset(args);
	cr_assert(return_value == BUILTIN_GENERIC_FAILURE);

	char expected_stderr[1024];
	sprintf(expected_stderr,
		"%s: unset: `%s`: not a valid identifier\n",
		ERROR_EMOJI, args[1]);

	cr_assert_stderr_eq_str(expected_stderr, "");
}

Test(builtin_unset,
	unset_multi_invalid_key_and_valid_one,
	.init = setup_empty_env)
{
	sheenv_store_from_key_value("TEST", "value");
	sheenv_store_from_key_value("TESTING", "value");
	char *args[] = {
		"unset",
		"1BLATEST",
		"TESTING",
		"25TESTING",
		NULL
	};

	cr_redirect_stderr();

	int return_value = builtin_unset(args);
	cr_assert(return_value == BUILTIN_GENERIC_FAILURE);

	char expected_stderr[1024];
	sprintf(expected_stderr,
		"%s: unset: `%s`: not a valid identifier\n"
		"%s: unset: `%s`: not a valid identifier\n",
		ERROR_EMOJI, args[1], ERROR_EMOJI, args[3]);

	cr_assert_stderr_eq_str(expected_stderr, "");
}
