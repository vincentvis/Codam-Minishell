
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

Test(builtin_export,
	export_without_args,
	.init = setup_empty_env)
{
	sheenv_store_from_key_value("test", "value");

	char *args[] = {
		"export",
		NULL
	};
	cr_redirect_stdout();

	int return_value = builtin_export(args);
	cr_assert(return_value == BUILTIN_SUCCESS);
	cr_assert_stdout_eq_str("declare -x test=\"value\"\n", "");
}

Test(builtin_export,
	export_with_empty_env,
	.init = setup_empty_env)
{
	char *args[] = {
		"export",
		NULL
	};
	cr_redirect_stdout();
	write(STDOUT_FILENO, "!", 1);

	int return_value = builtin_export(args);

	cr_assert(return_value == BUILTIN_SUCCESS);
	cr_assert_stdout_eq_str("!", "");
}

Test(builtin_export,
	export_in_lexographical_order,
	.init = setup_empty_env)
{
	sheenv_store_from_key_value("a_lowercase_last", "value");
	sheenv_store_from_key_value("A_uppercase_first", "value");
	sheenv_store_from_key_value("_underscore_in_them_middle", "value");

	char *args[] = {
		"export",
		NULL
	};
	cr_redirect_stdout();

	int return_value = builtin_export(args);
	cr_assert(return_value == BUILTIN_SUCCESS);
	cr_assert_stdout_eq_str("declare -x A_uppercase_first=\"value\"\n"
							"declare -x _underscore_in_them_middle=\"value\"\n"
							"declare -x a_lowercase_last=\"value\"\n", "");
}

Test(builtin_export,
	export_one_variable,
	.init = setup_empty_env)
{
	char *args[] = {
		"export",
		"TEST=value",
		NULL
	};

	int return_value = builtin_export(args);

	cr_assert(return_value == BUILTIN_SUCCESS);
	char *value = sheenv_get_value_by_key("TEST").data;
	cr_assert_str_eq(value, "value");
}

Test(builtin_export,
	export_two_variables,
	.init = setup_empty_env)
{
	char *args[] = {
		"export",
		"TEST=value",
		"ANOTHER=with value",
		NULL
	};

	int return_value = builtin_export(args);

	cr_assert(return_value == BUILTIN_SUCCESS);
	char *value1 = sheenv_get_value_by_key("TEST").data;
	char *value2 = sheenv_get_value_by_key("ANOTHER").data;
	cr_assert_str_eq(value1, "value");
	cr_assert_str_eq(value2, "with value");
}

Test(builtin_export,
	export_two_variables_last_invalid,
	.init = setup_empty_env)
{
	char *args[] = {
		"export",
		"TEST=value",
		"1ANOTHER=with value",
		NULL
	};

	cr_redirect_stderr();

	int return_value = builtin_export(args);

	cr_assert(return_value == BUILTIN_GENERIC_FAILURE);
	char *value1 = sheenv_get_value_by_key("TEST").data;
	cr_assert_str_eq(value1, "value");

	char expected_stderr[1024];
	sprintf(expected_stderr,
		"%s: export: `%s`: not a valid identifier\n",
		ERROR_EMOJI, args[2]);
	cr_assert_stderr_eq_str(expected_stderr, "");
}

Test(builtin_export,
	export_two_variables_first_invalid,
	.init = setup_empty_env)
{
	char *args[] = {
		"export",
		"1TEST=value",
		"ANOTHER=with value",
		NULL
	};

	cr_redirect_stderr();

	int return_value = builtin_export(args);

	cr_assert(return_value == BUILTIN_GENERIC_FAILURE);
	char *value1 = sheenv_get_value_by_key("ANOTHER").data;
	cr_assert_str_eq(value1, "with value");


	char expected_stderr[1024];
	sprintf(expected_stderr,
		"%s: export: `%s`: not a valid identifier\n",
		ERROR_EMOJI, args[1]);

	cr_assert_stderr_eq_str(expected_stderr, "");
}

Test(builtin_export,
	export_multiple_invalid,
	.init = setup_empty_env)
{
	char *args[] = {
		"export",
		"1TEST=value",
		"ANOTHER=with value",
		"3rd_also_invalid",
		NULL
	};

	cr_redirect_stderr();

	int return_value = builtin_export(args);

	cr_assert(return_value == BUILTIN_GENERIC_FAILURE);
	char *value1 = sheenv_get_value_by_key("ANOTHER").data;
	cr_assert_str_eq(value1, "with value");


	char expected_stderr[1024];
	sprintf(expected_stderr,
		"%s: export: `%s`: not a valid identifier\n"
		"%s: export: `%s`: not a valid identifier\n",
		ERROR_EMOJI, args[1], ERROR_EMOJI, args[3]);

	cr_assert_stderr_eq_str(expected_stderr, "");
}

Test(builtin_export,
	export_without_value,
	.init = setup_empty_env)
{
	char *args[] = {
		"export",
		"TEST",
		"TEST2=",
		NULL
	};

	int return_value = builtin_export(args);

	cr_assert(return_value == BUILTIN_SUCCESS);
	char *value1 = sheenv_get_value_by_key("TEST").data;
	char *value2 = sheenv_get_value_by_key("TEST2").data;
	cr_assert_str_eq(value1, "");
	cr_assert_str_eq(value2, "");
}
