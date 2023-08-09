
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../include/env/she_env.h"
#include "../include/env/she_env_internal.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../generic_test_helpers.h"
#include "she_env_testing.h"

Test(env,
	sheenv_check_pointer)
{
	t_env_base	*base0;
	t_env_base	*base1;

	base0 = sheenv();
	base1 = sheenv();
	cr_assert(base0 == sheenv());
	cr_assert(base1 == sheenv());
	cr_assert(base0 == base1);
}

Test(env,
	init_with_one_keyvalue,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	const char *test_env[] = {
		[0] = "VAR_NAME=value",
		[1] = NULL
	};
	size_t	expected_keys = 1 + NUM_KEYS_ADDED_BY_DEFAULT;
	size_t	expected_values = 1 + NUM_VALUES_ADDED_BY_DEFAULT;

	t_response	init_response = sheenv_init(test_env);
	t_env_base	*env_base;

	cr_assert(init_response.status == SUCCESS);
	env_base = init_response.data;

	cr_expect(env_base->initialised);
	cr_expect(env_base->count_keys == expected_keys);
	cr_expect(env_base->count_values == expected_values);
	cr_assert_not_null(env_base->head);
	cr_assert_not_null(env_base->tail);
	cr_assert(env_base->head != env_base->tail);
	cr_assert_not_null(env_base->head->data);
	cr_assert_str_eq(env_base->head->data, test_env[0]);
}

Test(env,
	init_with_no_keyvalues,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	const char *test_env[] = {
		[0] = NULL
	};

	size_t	expected_keys = NUM_KEYS_ADDED_BY_DEFAULT;
	size_t	expected_values = NUM_VALUES_ADDED_BY_DEFAULT;

	t_response	init_response = sheenv_init(test_env);
	t_env_base	*env_base;

	cr_assert(init_response.status == SUCCESS);
	env_base = init_response.data;
	cr_expect(env_base->initialised);
	cr_expect(env_base->count_values == expected_values);
	cr_expect(env_base->count_keys == expected_keys);
	cr_assert_not_null(env_base->head);
	cr_assert_not_null(env_base->head->data);
	cr_assert_not_null(env_base->tail);

	// assumes OLDPWD is added first
	cr_assert_str_eq(env_base->head->data, "OLDPWD");
}

Test(env,
	init_with_two_keyvalues,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	const char *test_env[] = {
		[0] = "VAR_NAME=value",
		[1] = "VAR_NAME2=value",
		[2] = NULL
	};
	size_t	expected_keys = 2 + NUM_KEYS_ADDED_BY_DEFAULT;
	size_t	expected_values = 2 + NUM_VALUES_ADDED_BY_DEFAULT;

	t_response	init_response = sheenv_init(test_env);
	t_env_base	*env_base;

	cr_assert(init_response.status == SUCCESS);
	env_base = init_response.data;
	cr_expect(env_base->initialised);
	cr_expect(env_base->count_keys == expected_keys);
	cr_expect(env_base->count_values == expected_values);

	t_env_var	*str = env_base->head;
	size_t			i = 0;
	while (test_env[i] && str)
	{
		cr_assert_not_null(str->data);
		cr_assert_str_eq(str->data, test_env[i]);
		i++;
		str = str->next;
	}
	// assumes OLDPWD is added first
	cr_assert_str_eq(str->data, "OLDPWD");
}

Test(env,
	check_key_as_part_of_input,
	.disabled = NOT_DISABLED
	)
{
	const char	*valid_keys[] = {
		"a", "a=", "a=b",
		"_", "_=", "_=b",
		"_m", "_m=", "_m=b",
		"_m", "_m=", "_m=b",
		"a1", "a1=", "a1=1",
		"F", "F=", "F=1",
		"F4", "F4=", "F4=1",
		"Fa4", "Fa4=", "Fa4=1",
		"A==x",
	};
	const char	*invalid_keys[] = {
		NULL, "",
		"=",  "=a",
		"1", "1=", "1=a",
		"1a", "1a=", "1a=a",
		"1a", "1a=", "1a=a",
		"$a", "$a=", "$a=a",
		"A-", "A-=", "A-=a",
		"A$", "A$=", "A$=a",
		"F(", "F(=", "F(=a",
	};
	const size_t num_valid_keys = sizeof(valid_keys) / sizeof(char*);
	size_t valid_key_count = 0;
	const size_t num_invalid_keys = sizeof(invalid_keys) / sizeof(char*);
	size_t invalid_key_count = 0;

	while (valid_key_count < num_valid_keys)
	{
		cr_expect(sheenv_is_invalid_key(valid_keys[valid_key_count], IS_PART_OF_INPUT) == false);
		valid_key_count++;
	}
	while (invalid_key_count < num_invalid_keys)
	{
		cr_expect(sheenv_is_invalid_key(invalid_keys[invalid_key_count], IS_PART_OF_INPUT) == true);
		invalid_key_count++;
	}
}

Test(env,
	check_key_separate_from_input,
	.disabled = NOT_DISABLED
	)
{
	const char	*valid_keys[] = {
		"a", "ab", "a1",
		"a_", "_a", "___",
		"A", "AA", "A_", "_A",
		"B3", "OOF",
	};
	const char	*invalid_keys[] = {
		NULL, ""
		"-", "a-", "1", "1a", "_-",
		"!", "FO(", "#A", "$a",
	};
	const size_t num_valid_keys = sizeof(valid_keys) / sizeof(char*);
	size_t valid_key_count = 0;
	const size_t num_invalid_keys = sizeof(invalid_keys) / sizeof(char*);
	size_t invalid_key_count = 0;

	while (valid_key_count < num_valid_keys)
	{
		cr_expect(sheenv_is_invalid_key(valid_keys[valid_key_count], IS_NOT_PART_OF_INPUT) == false);
		valid_key_count++;
	}
	while (invalid_key_count < num_invalid_keys)
	{
		cr_expect(sheenv_is_invalid_key(invalid_keys[invalid_key_count], IS_NOT_PART_OF_INPUT) == true);
		invalid_key_count++;
	}
}

Test(env,
	sheenv_cmp_string_with_key,
	.disabled = NOT_DISABLED)
{
	const struct data_pairs {
		const char	*data;
		const char	*key;
		size_t		key_length;
		bool		expected;
	} tests[] = {
		// these should return true
		{ .data = "a", .key = "a", .key_length = 1, .expected = true},
		{ .data = "same_without_eq", .key = "same_without_eq", .key_length = 15, .expected = true},
		{ .data = "same_with_eq=", .key = "same_with_eq=", .key_length = 12, .expected = true},
		{ .data = "same_with_eq_and_val=value", .key = "same_with_eq_and_val=value", .key_length = 20, .expected = true},

		// these should return false
		{ .data = "not_same", .key = "other_str", .key_length = 8, .expected = false},
		{ .data = "same_not_eof_or_eq-", .key = "same_not_eof_or_eq-", .key_length = 18, .expected = false},
		{ .data = "same_wrong_key_len", .key = "same_wrong_key_len", .key_length = 14, .expected = false},

		// NULL at the end so we know where to stop
		{ .data = NULL}
	};
	size_t	test_iterator = 0;

	while (tests[test_iterator].data != NULL)
	{
		cr_assert(sheenv_cmp_string_with_key(
			tests[test_iterator].data,
			tests[test_iterator].key,
			tests[test_iterator].key_length
		) == tests[test_iterator].expected);
		test_iterator++;
	}
}

Test(env,
	init_with_one_invalid_key,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	const char *test_env[] = {
		[0] = "1VAR_NAME=value",
		[1] = NULL
	};

	t_response	init_response = sheenv_init(test_env);

	cr_assert(init_response.status == SHEENV_BAD_INPUT);
	cr_assert_null(init_response.data);

	t_env_base	*env_base = sheenv();
	cr_assert_null(env_base->head);
	cr_assert_null(env_base->tail);
	cr_assert(env_base->initialised == false);
}

Test(env,
	init_with_second_invalid_key,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	const char *test_env[] = {
		[0] = "VAR_NAME=value",
		[1] = "VAR_ NAME=value",
		[2] = NULL
	};

	t_response	init_response = sheenv_init(test_env);

	cr_assert(init_response.status == SHEENV_BAD_INPUT);
	cr_assert_null(init_response.data);

	t_env_base	*env_base = sheenv();
	cr_assert_null(env_base->head);
	cr_assert_null(env_base->tail);
	cr_assert(env_base->initialised == false);
}

Test(env,
	init_twice,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	t_response	init_response = sheenv_init(fake_environ);

	cr_assert(init_response.status == SUCCESS);
	cr_assert_not_null(init_response.data);
	t_env_base	*env_base = sheenv();
	cr_assert_not_null(env_base->head);
	cr_assert_not_null(env_base->tail);
	cr_assert(env_base->initialised == true);

	t_response	doubled_init_response = sheenv_init(fake_environ);
	cr_assert(doubled_init_response.status == SHEENV_DOUBLE_INIT);
	cr_assert_null(doubled_init_response.data);

	t_env_base	*env_base_after_double_init = sheenv();
	cr_assert(env_base_after_double_init == env_base);
	cr_assert(env_base_after_double_init->head == env_base->head);
	cr_assert(env_base_after_double_init->tail == env_base->tail);
	cr_assert(env_base_after_double_init->initialised == true);
}

Test(env,
	validate_new_env_var_from_key,
	.disabled = NOT_DISABLED)
{
	const char		*key = "VALID_KEY";
	const size_t	key_len = strlen(key);
	t_response	response = sheenv_new_env_var_from_key(key);

	cr_assert(response.status == SUCCESS);
	cr_assert_not_null(response.data);

	t_env_var	*env_var = response.data;
	cr_assert_str_eq(env_var->data, key);
	cr_assert(env_var->key_length == key_len);
	cr_assert_null(env_var->next);
	cr_assert(env_var->data != key);
	// checks if it can be freed
	free(env_var->data);
	free(env_var);
}


Test(env,
	validate_new_env_var_from_key_value,
	.disabled = NOT_DISABLED)
{
	const char		*key = "VALID_KEY";
	const char		*val = "this is a VALUE with = equals sign because why not";
	const size_t	key_len = strlen(key);
	const size_t	val_len = strlen(val);
	t_response	response = sheenv_new_env_var_from_key_value(key, val);

	cr_assert(response.status == SUCCESS);
	cr_assert_not_null(response.data);

	t_env_var	*env_var = response.data;
	cr_assert(memcmp(env_var->data, key, key_len) == 0);
	cr_assert(env_var->data[key_len] == '=');
	cr_assert(memcmp(&env_var->data[key_len + 1], val, val_len) == 0);
	cr_assert(env_var->key_length == key_len);
	cr_assert_null(env_var->next);
	// checks if it can be freed
	free(env_var->data);
	free(env_var);
}

Test(env,
	validate_new_env_var_from_input,
	.disabled = NOT_DISABLED)
{
	const char		*input = "_A_VALID_key=With some valid value";
	const size_t	input_len = strlen(input);
	const size_t	key_len = 12;
	t_response		response = sheenv_new_env_var_from_input(input);

	cr_assert(response.status == SUCCESS);
	cr_assert_not_null(response.data);

	t_env_var	*env_var = response.data;
	cr_assert_str_eq(env_var->data, input);
	cr_assert(env_var->data[key_len] == '=');
	cr_assert(env_var->key_length == key_len);
	cr_expect(strlen(env_var->data) == input_len);
	cr_assert_null(env_var->next);
	free(env_var->data);
	free(env_var);
}

Test(env,
	array_size_on_init,
	.disabled = NOT_DISABLED)
{
	const struct tmp_tests {
		size_t expected_keys;
		size_t expected_values;
		const char *env[16];
	} tests[] = {
		{ .expected_keys = 2, .expected_values = 2, .env = { "a=b", "b=c", NULL }},
		{ .expected_keys = 3, .expected_values = 3, .env = { "a=b", "b=c", "d=f", NULL }},
		{ .expected_keys = 3, .expected_values = 3, .env = { "a=", "b=", "d=", NULL }},
		{ .expected_keys = 1, .expected_values = 0, .env = { "a", NULL }},
		{ .expected_keys = 1, .expected_values = 1, .env = { "a=", NULL }},
		{ .expected_keys = 1, .expected_values = 1, .env = { "a=a", NULL }},
		{ .expected_keys = 4, .expected_values = 2, .env = { "a=b", "b", "d", "e=f", NULL }},
		{ .expected_keys = 4, .expected_values = 3, .env = { "a=b", "b", "d=", "e=f", NULL }},
		{ .expected_keys = 2, .expected_values = 1, .env = { "a", "a", "a", "e=f", NULL }}
	};

	size_t	i = 0;
	size_t	max = 8;

	t_response	init_response;
	t_env_base	*env_base;
	while (i < max)
	{
		init_response = sheenv_init((const char **)tests[i].env);
		cr_expect(init_response.status == SUCCESS);
		cr_expect_not_null(init_response.data);
		env_base = sheenv();
		cr_expect(env_base->count_values == (tests[i].expected_values + NUM_VALUES_ADDED_BY_DEFAULT));
		cr_expect(env_base->count_keys == (tests[i].expected_keys + NUM_KEYS_ADDED_BY_DEFAULT));
		sheenv_destroy();
		i++;
	}
}

Test(env,
	to_array_No_empty_values,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	t_response	init_response = sheenv_init(fake_environ);
	cr_assert(init_response.status == SUCCESS);
	cr_assert_not_null(init_response.data);

	t_response	to_array = sheenv_to_array(HANDLE_EMPTY_VALUES);
	cr_assert(to_array.status == SUCCESS);
	cr_assert_not_null(to_array.data);
	char 		**array = to_array.data;
	size_t		count = 0;

	while (count < NUM_ELEMS_IN_FAKE_ENVIRON)
	{
		cr_assert_str_eq(array[count], fake_environ[count]);
		count++;
	}
	free(array);
}

Test(env,
	to_array_With_empty_values,
	.disabled = NOT_DISABLED)
{
	const char *env[] = {
		[0] = "FIRST_VAR=Value with spaces",
		[1] = "__A=double underscore",
		[2] = "EMPTY",
		[3] = "ONLY_EQ=",
		[4] = NULL
	};
	// size_t	expected_keys = 3 + NUM_KEYS_ADDED_BY_DEFAULT;
	// size_t	expected_values = 3;
	t_response	init_response = sheenv_init(env);
	cr_assert(init_response.status == SUCCESS);
	cr_assert_not_null(init_response.data);

	t_response	to_array_handle_empty = sheenv_to_array(HANDLE_EMPTY_VALUES);
	t_response	to_array_skip___empty = sheenv_to_array(SKIP_EMPTY_VALUES);

	cr_assert(to_array_handle_empty.status == SUCCESS);
	cr_assert_not_null(to_array_handle_empty.data);
	cr_assert(to_array_skip___empty.status == SUCCESS);
	cr_assert_not_null(to_array_skip___empty.data);

	char	**array_handle_empty = to_array_handle_empty.data;
	char	**array_skip___empty = to_array_skip___empty.data;

	// first two should be the same
	cr_assert_str_eq(array_handle_empty[0], env[0]);
	cr_assert_str_eq(array_handle_empty[1], env[1]);
	cr_assert_str_eq(array_skip___empty[0], env[0]);
	cr_assert_str_eq(array_skip___empty[1], env[1]);

	// if we skip the empty ones, the 3rd elem should be the 4th from init
	cr_assert_str_eq(array_skip___empty[2], env[3]);

	// if we do not skip empty, it should just contain everything
	cr_assert_str_eq(array_handle_empty[2], env[2]);
	cr_assert_str_eq(array_handle_empty[3], env[3]);
}

Test(env,
	print_sorted,
	.disabled = NOT_DISABLED)
{
	// setup the env, in this case in a sorted list but with indexies scrambled
	// because that was easier for figureing out the sorted list afterwards..
	const char *env[] = {
		[4] = "A_APPEAR_FIRST=value",
		[2] = "B_APPEAR_SECOND=value",
		[3] = "C_APPEAR_MIDDLE=value",
		[0] = "_SHOULD_AFTER_UPPERCASE=value",
		[5] = "lowercase=value",
		[1] = "xlowercase=value",
		[6] = NULL
	};

	// here just the whole output as expected, newsline separated..
	const char *expected_content = ""
		"declare -x A_APPEAR_FIRST=\"value\"\n"
		"declare -x B_APPEAR_SECOND=\"value\"\n"
		"declare -x C_APPEAR_MIDDLE=\"value\"\n"
		"declare -x OLDPWD\n"
		"declare -x SHLVL=\"1\"\n"
		"declare -x _SHOULD_AFTER_UPPERCASE=\"value\"\n"
		"declare -x lowercase=\"value\"\n"
		"declare -x xlowercase=\"value\"\n"
		;

	t_response init_response = sheenv_init(env);
	cr_assert(init_response.status == SUCCESS);

	cr_redirect_stdout();

	int	print_export_response = sheenv_print_export();
	cr_expect(print_export_response == SUCCESS);
	cr_expect_stdout_eq_str(expected_content, "");

	sheenv_destroy();
}

Test(env,
	unset_existing_head,
	.disabled = NOT_DISABLED)
{
	t_response	init_response = sheenv_init(fake_environ);
	cr_assert(init_response.status == SUCCESS);
	cr_assert_not_null(init_response.data);
	t_env_base	*init_base = init_response.data;
	t_env_var	*second_var_on_init = init_base->head->next;

	t_response	unset_head = sheenv_unset("FIRST_VAR");
	cr_assert(unset_head.status == SUCCESS);
	cr_assert_null(unset_head.data);

	t_env_base	*env_base = sheenv();
	cr_assert(second_var_on_init == env_base->head);
	cr_assert_str_eq(env_base->head->data, fake_environ[1]);

	sheenv_destroy();
}

Test(env,
	unset_existing_not_head,
	.disabled = NOT_DISABLED)
{
	t_response	init_response = sheenv_init(fake_environ);
	cr_assert(init_response.status == SUCCESS);
	cr_assert_not_null(init_response.data);
	t_env_base	*init_base = init_response.data;

	t_response	unset_head = sheenv_unset("__A");
	cr_assert(unset_head.status == SUCCESS);
	cr_assert_null(unset_head.data);

	t_env_base	*env_base = sheenv();
	cr_assert(init_base->head == env_base->head);
	cr_assert_str_eq(env_base->head->data, fake_environ[0]);
	cr_assert_str_eq(env_base->head->next->data, fake_environ[2]);

	sheenv_destroy();
}

Test(env,
	unset_and_try_to_free,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	t_response	init_response = sheenv_init(fake_environ);
	cr_assert(init_response.status == SUCCESS);
	cr_assert_not_null(init_response.data);
	t_env_base	*init_base = init_response.data;

	t_env_var	*second_var	= init_base->head->next;
	// char		*second_var_data = second_var->data;

	t_response	unset_second_var = sheenv_unset("__A");
	cr_assert(unset_second_var.status == SUCCESS);
	// str should not be equal anymore... but this might be undefined behaviour
	// since the data is never overwritten..
	// but free'ing again here does not cause SIGSEGV signal but some boxfort-worker error
	cr_assert_str_neq(second_var->data, fake_environ[1]);

	t_response	unset_second_var_again = sheenv_unset("__A");
	cr_assert(unset_second_var_again.status == SHEENV_KEY_NOT_FOUND);
}

Test(env,
	store_existing_key_should_update_value,
	.init = setup_fake_env,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	const char	*new_str = "ABC=enw_value";
	t_response	store_existing = sheenv_store_from_input(new_str);
	cr_assert(store_existing.status == SUCCESS);
	cr_assert_not_null(store_existing.data);

	t_env_base	*base = sheenv();
	t_env_var	*tail = base->tail;
	cr_assert_str_eq(tail->data, "SHLVL=1");
	t_env_var	*target_elem_that_should_be_updated = base->head;
	size_t		i = 0;
	while (i < (NUM_ELEMS_IN_FAKE_ENVIRON - 1))
	{
		target_elem_that_should_be_updated = target_elem_that_should_be_updated->next;
		i++;
	}
	cr_assert_str_eq(target_elem_that_should_be_updated->data, new_str);
	cr_assert_str_eq(((t_env_var *)store_existing.data)->data, new_str);
}

Test(env,
	store_new_value,
	.init = setup_fake_env,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	t_env_var	*tail_before_store = sheenv()->tail;
	const char	*new_var = "a_new_key=with a value";
	t_response	store_response = sheenv_store_from_input(new_var);

	cr_assert(store_response.status == SUCCESS);
	cr_assert_not_null(store_response.data);

	t_env_base	*base = sheenv();
	t_env_var	*stored_var = store_response.data;
	cr_assert(base->tail == stored_var);
	cr_assert(base->count_keys == NUM_ELEMS_IN_FAKE_ENVIRON + NUM_KEYS_ADDED_BY_DEFAULT + 1);
	cr_assert(base->count_values == NUM_ELEMS_IN_FAKE_ENVIRON + NUM_VALUES_ADDED_BY_DEFAULT + 1);
	cr_assert_str_eq(base->tail->data, new_var);
	cr_assert_not_null(tail_before_store->next);
	cr_assert_str_eq(tail_before_store->next->data, new_var);
	cr_assert_str_eq(tail_before_store->next->data, base->tail->data);
}

Test(env,
	get_existing_by_key,
	.init = setup_fake_env,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	t_response get_struct = sheenv_get_struct_by_key("ABC");
	t_response get_value = sheenv_get_value_by_key("ABC");

	cr_assert(get_struct.status == SUCCESS);
	cr_assert(get_value.status == SUCCESS);

	cr_assert_not_null(get_struct.data);
	cr_assert_not_null(get_value.data);

	t_env_var	*var_struct = get_struct.data;
	char		*var_value = get_value.data;

	cr_assert_str_eq(var_struct->data, fake_environ[3]);
	cr_assert_str_eq(var_value, "DEF");

	// check that we are looking at the same data
	cr_assert(&var_struct->data[var_struct->key_length + 1] == var_value);
}

Test(env,
	get_existing_by_input,
	.init = setup_fake_env,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	t_response get_struct = sheenv_get_struct_by_input("ABC=something_else");
	t_response get_value = sheenv_get_value_by_input("ABC=something_else");

	cr_assert(get_struct.status == SUCCESS);
	cr_assert(get_value.status == SUCCESS);

	cr_assert_not_null(get_struct.data);
	cr_assert_not_null(get_value.data);

	t_env_var	*var_struct = get_struct.data;
	char		*var_value = get_value.data;

	cr_assert_str_eq(var_struct->data, fake_environ[3]);
	cr_assert_str_eq(var_value, "DEF");

	// check that we are looking at the same data
	cr_assert(&var_struct->data[var_struct->key_length + 1] == var_value);
}

Test(env,
	get_non_existing_by_key,
	.init = setup_fake_env,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	t_response get_struct = sheenv_get_struct_by_key("FOOOOOOOOO");
	t_response get_value = sheenv_get_value_by_key("FOOOOOOOOO");

	cr_assert(get_struct.status == SHEENV_KEY_NOT_FOUND);
	cr_assert(get_value.status == SHEENV_KEY_NOT_FOUND);

	cr_assert_null(get_struct.data);
	cr_assert_null(get_value.data);
}

Test(env,
	get_non_existing_by_input,
	.init = setup_fake_env,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	t_response get_struct = sheenv_get_struct_by_input("FOOOOOOOOO=x");
	t_response get_value = sheenv_get_value_by_input("FOOOOOOOOO=x");

	cr_assert(get_struct.status == SHEENV_KEY_NOT_FOUND);
	cr_assert(get_value.status == SHEENV_KEY_NOT_FOUND);

	cr_assert_null(get_struct.data);
	cr_assert_null(get_value.data);
}

Test(env,
	get_with_bad_input,
	.init = setup_fake_env,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	t_response get_struct_key = sheenv_get_struct_by_key("1dxx");
	t_response get_value_key = sheenv_get_value_by_key("1dxx");
	t_response get_struct_input = sheenv_get_struct_by_input("1xd=x");
	t_response get_value_input = sheenv_get_value_by_input("1dx=x");

	cr_assert(get_struct_key.status == SHEENV_BAD_INPUT);
	cr_assert(get_value_key.status == SHEENV_BAD_INPUT);
	cr_assert(get_struct_input.status == SHEENV_BAD_INPUT);
	cr_assert(get_value_input.status == SHEENV_BAD_INPUT);

	cr_assert_null(get_struct_key.data);
	cr_assert_null(get_value_key.data);
	cr_assert_null(get_struct_input.data);
	cr_assert_null(get_value_input.data);
}

Test(env,
	print_without_empty,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	const char *env[] = {
		"CBA=abc",
		"x=:ddd",
		"F1",
		"f3=",
		NULL
	};
	t_response init = sheenv_init(env);
	cr_assert(init.status == SUCCESS);

	const char	*expected = ""
		"CBA=abc\n"
		"x=:ddd\n"
		"f3=\n"
		"SHLVL=1\n"
		"CBA=abc\n"
		"x=:ddd\n"
		"F1\n"
		"f3=\n"
		"OLDPWD\n"
		"SHLVL=1\n"
		;

	cr_redirect_stdout();

	sheenv_print(SKIP_EMPTY_VALUES);
	sheenv_print(HANDLE_EMPTY_VALUES);

	cr_expect_stdout_eq_str(expected, "");
}

Test(env,
	update_without_value_should_not_unset_value,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	const char	*tmp_env[] = {
		[0] = "TEST=123",
		[1] = NULL
	};
	size_t start_size = 1 + NUM_KEYS_ADDED_BY_DEFAULT;
	t_response	init = sheenv_init(tmp_env);
	cr_assert(init.status == SUCCESS);
	cr_assert_not_null(init.data);
	cr_assert(sheenv()->count_keys == start_size);
	cr_assert(sheenv()->count_values == 1 + NUM_VALUES_ADDED_BY_DEFAULT);

	t_response	store = sheenv_store_from_input("TEST");
	t_env_var	*store_var = store.data;
	cr_assert(store.status == SUCCESS);
	cr_expect_str_eq(store_var->data, tmp_env[0]);
	cr_expect_str_eq(sheenv()->head->data, tmp_env[0]);
	cr_assert(sheenv()->count_keys == start_size);
	cr_assert(sheenv()->count_values == 1 + NUM_VALUES_ADDED_BY_DEFAULT);
}

Test(env,
	update_with_equals_but_no_value_should_set_empty_value,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	const char	*tmp_env[] = {
		[0] = "TEST=123",
		[1] = NULL
	};
	t_response	init_response = sheenv_init(tmp_env);
	cr_assert(init_response.status == SUCCESS);
	cr_assert_not_null(init_response.data);

	t_response	update_response = sheenv_store_from_input("TEST=");
	cr_assert(update_response.status == SUCCESS);
	cr_assert_not_null(update_response.data);
	t_env_var	*updated_var = update_response.data;
	cr_assert_not_null(updated_var->data);
	cr_assert_str_eq(updated_var->data, "TEST=");
}

Test(env,
	check_size_update_in_base_struct,
	.fini = cleanup_after,
	.disabled = NOT_DISABLED)
{
	const char	*tmp_env[] = {
		[0] = "VAR=123",
		[1] = NULL
	};

	t_response	init = sheenv_init(tmp_env);
	cr_assert(init.status == SUCCESS);
	cr_assert_not_null(init.data);

	make_sure_env_is_empty();
	cr_assert_null(sheenv()->head);
	cr_assert_null(sheenv()->tail);

	const struct test_data {
		const char	*string;
		size_t		expected_keys_after;
		size_t		expected_values_after;
		t_response (*func)(const char *str);
	} tests[] = {
		// these should return true
		{ .string = "VAR", 					.func = &sheenv_store_from_input, 	.expected_keys_after = 1, .expected_values_after = 0},
		{ .string = "TEST=value", 			.func = &sheenv_store_from_input, 	.expected_keys_after = 2, .expected_values_after = 1},
		{ .string = "TEST=different value", .func = &sheenv_store_from_input, 	.expected_keys_after = 2, .expected_values_after = 1},
		{ .string = "NEW=bal", 				.func = &sheenv_store_from_input, 	.expected_keys_after = 3, .expected_values_after = 2},
		{ .string = "TEST", 				.func = &sheenv_unset, 				.expected_keys_after = 2, .expected_values_after = 1},
		{ .string = "TEST", 				.func = &sheenv_unset, 				.expected_keys_after = 2, .expected_values_after = 1},
		{ .string = "NEW", 					.func = &sheenv_unset, 				.expected_keys_after = 1, .expected_values_after = 0},
		{ .string = "VAR", 					.func = &sheenv_unset, 				.expected_keys_after = 0, .expected_values_after = 0},
		{ .string = "NON_EXISTENT",			.func = &sheenv_unset, 				.expected_keys_after = 0, .expected_values_after = 0},
		{ .string = "VAR",					.func = &sheenv_unset,			 	.expected_keys_after = 0, .expected_values_after = 0},
		{ .string = "VAR",					.func = &sheenv_store_from_input, 	.expected_keys_after = 1, .expected_values_after = 0},
		{ .string = "VAR=value",			.func = &sheenv_store_from_input, 	.expected_keys_after = 1, .expected_values_after = 1},
		{ .string = "VAR=value",			.func = &sheenv_store_from_input, 	.expected_keys_after = 1, .expected_values_after = 1},
		{ .string = "VAR1=value",			.func = &sheenv_store_from_input, 	.expected_keys_after = 2, .expected_values_after = 2},
		{ .string = "VAR1=value",			.func = &sheenv_store_from_input, 	.expected_keys_after = 2, .expected_values_after = 2},
		{ .string = "VAR2=value",			.func = &sheenv_store_from_input, 	.expected_keys_after = 3, .expected_values_after = 3},
		{ .string = "VAR",					.func = &sheenv_unset,			 	.expected_keys_after = 2, .expected_values_after = 2},
		{ .string = NULL }
	};
	size_t	i = 0;


	t_env_base *base = sheenv();
	while (tests[i].string)
	{
		tests[i].func(tests[i].string);
		base = sheenv();
		cr_expect(base->count_keys == tests[i].expected_keys_after,
			"fail on count_keys[%lu], keys: %lu, expected: %lu",
			i,
			base->count_keys,
			tests[i].expected_keys_after);

		cr_expect(base->count_values == tests[i].expected_values_after,
			"fail on count_values[%lu], values: %lu, expected: %lu",
			i,
			base->count_values,
			tests[i].expected_values_after);
		i++;
	}
}

Test(env,
	destroy_should_zeroout_values,
	.init = setup_fake_env,
	.disabled = NOT_DISABLED)
{
	t_env_base *env_base = sheenv();
	cr_assert(env_base->initialised == true);
	cr_assert_not_null(env_base->head);
	cr_assert_not_null(env_base->tail);
	cr_assert(env_base->count_keys != 0);
	cr_assert(env_base->count_values != 0);

	sheenv_destroy();

	cr_assert(env_base->initialised == false);
	cr_assert_null(env_base->head);
	cr_assert_null(env_base->tail);
	cr_assert(env_base->count_keys == 0);
	cr_assert(env_base->count_values == 0);
}

Test(env,
	make_sure_shlvl_is_set)
{
	const char *empty[] = {
		NULL
	};

	t_response init = sheenv_init(empty);

	cr_assert(init.status == SUCCESS);
	cr_assert_not_null(init.data);

	t_response oldpwd_response = sheenv_get_struct_by_key("OLDPWD");
	cr_assert(oldpwd_response.status == SUCCESS);
	cr_assert_not_null(oldpwd_response.data);

	t_env_var *oldpwd = oldpwd_response.data;
	cr_assert_str_eq(oldpwd->data, "OLDPWD");
	cr_assert(oldpwd->key_length == strlen("OLDPWD"));
	cr_assert(oldpwd->key_length == strlen(oldpwd->data));

	sheenv_destroy();
}

Test(env,
	make_sure_oldpwd_is_overwritten)
{
	const char *empty[] = {
		"OLDPWD=something",
		NULL
	};

	t_response init = sheenv_init(empty);

	cr_assert(init.status == SUCCESS);
	cr_assert_not_null(init.data);

	t_response oldpwd_response = sheenv_get_struct_by_key("OLDPWD");
	cr_assert(oldpwd_response.status == SUCCESS);
	cr_assert_not_null(oldpwd_response.data);

	t_env_var *oldpwd = oldpwd_response.data;
	cr_assert_str_eq(oldpwd->data, "OLDPWD");
	cr_assert(oldpwd->key_length == strlen("OLDPWD"));
	cr_assert(oldpwd->key_length == strlen(oldpwd->data));

	sheenv_destroy();
}

Test(env,
	shlvl_should_be_set_to_one_if_not_existing)
{
	const char *empty[] = {
		NULL
	};

	t_response init = sheenv_init(empty);

	cr_assert(init.status == SUCCESS);
	cr_assert_not_null(init.data);

	t_response shlvl_response = sheenv_get_struct_by_key("SHLVL");
	cr_assert(shlvl_response.status == SUCCESS);
	cr_assert_not_null(shlvl_response.data);

	t_env_var *shlvl = shlvl_response.data;
	// check if it is set to 1
	cr_assert_str_eq(shlvl->data, "SHLVL=1");

	sheenv_destroy();
}

Test(env,
	shlvl_should_be_increased_by_one_if_below_1000)
{
	const char *empty[] = {
		"SHLVL=1",
		NULL
	};

	t_response init = sheenv_init(empty);

	cr_assert(init.status == SUCCESS);
	cr_assert_not_null(init.data);

	t_response shlvl_response = sheenv_get_struct_by_key("SHLVL");
	cr_assert(shlvl_response.status == SUCCESS);
	cr_assert_not_null(shlvl_response.data);

	t_env_var *shlvl = shlvl_response.data;
	// check if it is set to 1
	cr_assert_str_eq(shlvl->data, "SHLVL=2");

	sheenv_destroy();
}

Test(env,
	shlvl_should_be_reset_to_1_if_1000)
{
	const char *empty[] = {
		"SHLVL=1000",
		NULL
	};

	t_response init = sheenv_init(empty);

	cr_assert(init.status == SUCCESS);
	cr_assert_not_null(init.data);

	t_response shlvl_response = sheenv_get_struct_by_key("SHLVL");
	cr_assert(shlvl_response.status == SUCCESS);
	cr_assert_not_null(shlvl_response.data);

	t_env_var *shlvl = shlvl_response.data;
	// check if it is set to 1
	cr_assert_str_eq(shlvl->data, "SHLVL=1");

	sheenv_destroy();
}

Test(env,
	shlvl_should_be_reset_to_1_if_above_1000)
{
	const char *empty[] = {
		"SHLVL=1245",
		NULL
	};

	t_response init = sheenv_init(empty);

	cr_assert(init.status == SUCCESS);
	cr_assert_not_null(init.data);

	t_response shlvl_response = sheenv_get_struct_by_key("SHLVL");
	cr_assert(shlvl_response.status == SUCCESS);
	cr_assert_not_null(shlvl_response.data);

	t_env_var *shlvl = shlvl_response.data;
	// check if it is set to 1
	cr_assert_str_eq(shlvl->data, "SHLVL=1");

	sheenv_destroy();
}

Test(env,
	shlvl_should_be_reset_to_empty_if_999)
{
	const char *empty[] = {
		"SHLVL=999",
		NULL
	};

	t_response init = sheenv_init(empty);

	cr_assert(init.status == SUCCESS);
	cr_assert_not_null(init.data);

	t_response shlvl_response = sheenv_get_struct_by_key("SHLVL");
	cr_assert(shlvl_response.status == SUCCESS);
	cr_assert_not_null(shlvl_response.data);

	t_env_var *shlvl = shlvl_response.data;
	// check if it is set to 1
	cr_assert_str_eq(shlvl->data, "SHLVL=");

	sheenv_destroy();
}
