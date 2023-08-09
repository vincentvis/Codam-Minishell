#include <criterion/criterion.h>
#include "../include/utils/ifs.h"
#include "env/she_env.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TEST_IFS_DEFAULT_VALUE " \t\n"
#define TEST_NEW_IFS_VALUE "abc"

Test(ifs, check_defaults)
{
	cr_expect_str_eq(DEFAULT_IFS, TEST_IFS_DEFAULT_VALUE);
	cr_expect_str_eq(get_ifs(IFS_GET), TEST_IFS_DEFAULT_VALUE);
	cr_expect_str_eq(get_ifs(IFS_TRY_UPDATE), TEST_IFS_DEFAULT_VALUE);
}

Test(ifs, set_to_somethin_else_form_env)
{
	const char *env[] = {
		"IFS=" TEST_NEW_IFS_VALUE,
		NULL
	};
	t_response sheenv = sheenv_init(env);
	cr_assert(sheenv.status == SUCCESS, "Stop test, env fails.. fix that first");
	cr_expect_str_eq(get_ifs(IFS_GET), TEST_NEW_IFS_VALUE);
	sheenv_destroy();
}

Test(ifs, set_to_somethin_dynamic)
{
	const char *env[] = {
		NULL
	};
	t_response sheenv = sheenv_init(env);
	cr_assert(sheenv.status == SUCCESS, "Stop test, env fails.. fix that first");
	cr_expect_str_eq(get_ifs(IFS_GET), TEST_IFS_DEFAULT_VALUE);
	sheenv_store_from_key_value("IFS", TEST_NEW_IFS_VALUE);
	cr_expect_str_eq(get_ifs(IFS_GET), TEST_IFS_DEFAULT_VALUE, "Value should not have changed until update is requested");
	cr_expect_str_eq(get_ifs(IFS_TRY_UPDATE), TEST_NEW_IFS_VALUE);
	sheenv_destroy();
}

Test(ifs, set_to_empty_string)
{
	const char *env[] = {
		"IFS=",
		NULL
	};
	t_response sheenv = sheenv_init(env);
	cr_assert(sheenv.status == SUCCESS, "Stop test, env fails.. fix that first");
	cr_expect_str_eq(get_ifs(IFS_GET), "");
	sheenv_destroy();
}
