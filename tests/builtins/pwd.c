
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "builtins/builtins.h"
#include <limits.h>

#include "env/she_env.h"
#include "../generic_test_helpers.h"
#include "builtins/builtins_internal.h"
#include "utils/readline.h"

#include <unistd.h>

Test(builtin_pwd,
	print_with_pwd_set_to_something_else,
	.init = setup_empty_env)
{
	chdir("/Users");
	sheenv_store_from_key_value("PWD", "/some_value");

	cr_redirect_stdout();

	int	return_value = builtin_pwd(NULL);

	cr_assert(return_value == BUILTIN_SUCCESS);
	cr_assert_stdout_eq_str("/Users\n", "");
}

Test(builtin_pwd,
	print_with_pwd_unset,
	.init = setup_empty_env)
{
	chdir("/Users");
	cr_redirect_stdout();

	int	return_value = builtin_pwd(NULL);

	cr_assert(return_value == BUILTIN_SUCCESS);
	cr_assert_stdout_eq_str("/Users\n", "");
}
