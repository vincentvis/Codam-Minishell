#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "builtins/builtins.h"
#include <limits.h>

#include <stdio.h>
#include "env/she_env.h"
#include "../generic_test_helpers.h"
#include "builtins/builtins_internal.h"
#include "utils/readline.h"

#include <unistd.h>

Test(builtin_env,
	prints_to_stdout_in_order_of_adding,
	.init = setup_empty_env)
{
	sheenv_store_from_key_value("First_key_to_be_added", "value");
	sheenv_store_from_key_value("second_key", "value");
	sheenv_store_from_key_value("_something", "value");
	sheenv_store_from_key_value("AA_starts_with_a_but_is_last", "value");

	char *args[] = {
		"env",
		NULL
	};
	cr_redirect_stdout();

	int return_value = builtin_env(args);

	cr_assert(return_value == BUILTIN_SUCCESS);
	cr_assert_stdout_eq_str("First_key_to_be_added=value\n"
							"second_key=value\n"
							"_something=value\n"
							"AA_starts_with_a_but_is_last=value\n", "");
}

Test(builtin_env,
	empty_env,
	.init = setup_empty_env)
{

	char *args[] = {
		"env",
		NULL
	};
	cr_redirect_stdout();

	int return_value = builtin_env(args);

	write(STDOUT_FILENO, "!", 1);
	cr_assert(return_value == BUILTIN_SUCCESS);
	cr_assert_stdout_eq_str("!", "");
}
