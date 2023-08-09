
#include <criterion/criterion.h>
#include <criterion/parameterized.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>

#include "debug/minishell_debug.nonorm"
#include "debug/fail_alloc.hh"

#include "lexer/token.h"
#include "env/she_env.h"
#include "expansion/expansion.h"
#include "expansion/expand_validation.h"
#include "expansion/expand_functions.h"

#include "expansion_helpers.h"

#undef DO_ALLOC_FAILING

#ifdef DO_ALLOC_FAILING
extern bool	should_malloc_fail;
extern bool	should_calloc_fail;
extern bool	should_substr_fail;
extern bool	should_strdup_fail;
#else
bool	should_malloc_fail;
bool	should_calloc_fail;
bool	should_substr_fail;
bool	should_strdup_fail;
#endif


struct alloc_fail {
	t_token		*input;
	t_response	(*f)(t_token *token);
	bool		*alloc_to_fail;
	int			fail_at;
};

bool	should_alloc_fail(bool should_fail, int action)
{
	static int	i = 0;
	static int	fail_at = -1;;

	if (should_fail == false && action == 1)
	{
		i = 0;
		fail_at = -1;
		return (true);
	}
	if (should_fail == false && action > 1)
	{
		i = 0;
		fail_at = action;
		return (true);
	}

	if (should_fail == false)
		return (false);

	i++;
	if (should_fail == true && fail_at == -1)
		return (true);

	if (fail_at == i)
		return (true);

	return (false);
}

#ifdef DO_ALLOC_FAILING
Test(expand_functions, alocation_failing)
{
	struct alloc_fail tests[] = {
		 { .input = make_token(DQUOTE_TYPE, "\"test 1\"", AS_LITERAL), .f = &expand_dquote_type, .alloc_to_fail = &should_substr_fail, .fail_at = 1},
		 { .input = make_token(DQUOTE_TYPE, "\"test 1\"", AS_LITERAL), .f = &expand_dquote_type, .alloc_to_fail = &should_calloc_fail, .fail_at = 1},
		 { .input = make_token(DQUOTE_TYPE, "\"$HOME\"", AS_LITERAL), .f = &expand_dquote_type, .alloc_to_fail = &should_substr_fail, .fail_at = 2},
		 { .input = make_token(DQUOTE_TYPE, "\"$HOME\"", AS_LITERAL), .f = &expand_dquote_type, .alloc_to_fail = &should_strdup_fail, .fail_at = 1},

		 { .input = make_token(VAR_TYPE, "$HOME", AS_LITERAL), .f = &expand_var_type, .alloc_to_fail = &should_substr_fail, .fail_at = 1},
		 { .input = make_token(VAR_TYPE, "$HOME", AS_LITERAL), .f = &expand_var_type, .alloc_to_fail = &should_strdup_fail, .fail_at = 1},

		 { .input = make_token(WORD_TYPE, "word", AS_LITERAL), .f = &expand_word_type, .alloc_to_fail = &should_substr_fail, .fail_at = 1},

		 { .f = NULL }
	};
	int i = 0;


	t_response	response;

	while (tests[i].f)
	{
		// reset static singleton stuff
		should_alloc_fail(false, 1);

		should_malloc_fail = false;
		should_calloc_fail = false;
		should_substr_fail = false;
		should_strdup_fail = false;

		// if we want to fail at a specific point, set that points
		if (tests[i].fail_at > 1)
			should_alloc_fail(false, tests[i].fail_at);

		*(tests[i].alloc_to_fail) = true;

		response = tests[i].f(tests[i].input);

		cr_expect(response.status == MALLOC_FAILURE, "Response status is not alloc failure [%i]", i);
		cr_expect(response.data == NULL, "Response data is not NULL after potentional alloc failure [%i]", i);

		i++;
	}
}
#endif

ParameterizedTestParameters(expand_functions, all)
{
	const int max_tests = 50;
	static t_token_content_expansion_test tests[max_tests + 1];

	size_t i = 0;

	// word type
	tests[i++] = (t_token_content_expansion_test){
		.input = param_make_token(WORD_TYPE, "word token", MAKE_COPY),
		.expected_output = param_make_token(WORD_TYPE, "word token", MAKE_COPY),
		.expected_status = SUCCESS,
		.func = 'w',
		.in_place_modification = true,
		.setup = 0, .teardown = 0,
	};

	tests[i++] = (t_token_content_expansion_test){
		.input = param_make_token(WORD_TYPE, "", MAKE_COPY),
		.expected_output = param_make_token(WORD_TYPE, "", MAKE_COPY),
		.func = 'w',
		.in_place_modification = true,
		.setup = 0, .teardown = 0,
	};

	// quote type
	tests[i++] = (t_token_content_expansion_test){
		.input = param_make_token(QUOTE_TYPE, "'single quoted string'", MAKE_COPY),
		.expected_output = param_make_token(QUOTE_TYPE, "single quoted string", MAKE_COPY),
		.func = '\'',
		.in_place_modification = true,
		.setup = 0, .teardown = 0,
	};

	tests[i++] = (t_token_content_expansion_test){
		.input = param_make_token(QUOTE_TYPE, "ssingle quoted string without the quotess", MAKE_COPY),
		.expected_output = param_make_token(QUOTE_TYPE, "single quoted string without the quotes", MAKE_COPY),
		.func = '\'',
		.in_place_modification = true,
		.setup = 0, .teardown = 0,
	};

	// tilde type
	tests[i++] = (t_token_content_expansion_test){
		.input = param_make_token(TILDE_TYPE, "~", MAKE_COPY),
		.expected_output = param_make_token(WORD_TYPE, TEST_HOME_DIR, MAKE_COPY),
		.func = '~',
		.in_place_modification = true,
		.setup = 1, .teardown = 1,
	};

	// exitcode type
	tests[i++] = (t_token_content_expansion_test){
		.input = param_make_token(EXITCODE_TYPE, "$?", AS_LITERAL),
		.expected_output = param_make_token(WORD_TYPE, "10", MAKE_COPY),
		.func = 'e',
		.in_place_modification = true,
		.setup = 2, .teardown = 2,
	};

	// dquote type
	tests[i++] = (t_token_content_expansion_test){
		.input = param_make_token(DQUOTE_TYPE, "\"a $HOME b\"", MAKE_COPY),
		.expected_output = param_make_token(DQUOTE_TYPE, "a " TEST_HOME_DIR " b", MAKE_COPY),
		.func = '"',
		.in_place_modification = true,
		.setup = 1, .teardown = 1,
	};

	tests[i++] = (t_token_content_expansion_test){
		.input = param_make_token(DQUOTE_TYPE, "\"$HOME\"", MAKE_COPY),
		.expected_output = param_make_token(DQUOTE_TYPE, TEST_HOME_DIR, MAKE_COPY),
		.func = '"',
		.in_place_modification = true,
		.setup = 1, .teardown = 1,
	};

	tests[i++] = (t_token_content_expansion_test){
		.input = param_make_token(DQUOTE_TYPE, "\"$USER'__'$SHELL\"", AS_LITERAL),
		.expected_output = param_make_token(DQUOTE_TYPE, TEST_USERNAME "'__'" TEST_SHELL_NAME, MAKE_COPY),
		.func = '"',
		.in_place_modification = true,
		.setup = 1, .teardown = 1,
	};

	tests[i++] = (t_token_content_expansion_test){
		.input = param_make_token(DQUOTE_TYPE, "\"$DLR$DLR\"", AS_LITERAL),
		.expected_output = param_make_token(DQUOTE_TYPE, "DLR $DLR $", MAKE_COPY),
		.func = '"',
		.in_place_modification = true,
		.setup = 1, .teardown = 1,
	};

	// var type
	tests[i++] = (t_token_content_expansion_test){
		.input = param_make_token(VAR_TYPE, "$HOME", AS_LITERAL),
		.expected_output = param_make_token(WORD_TYPE, TEST_HOME_DIR, MAKE_COPY),
		.func = '$',
		.in_place_modification = true,
		.setup = 1, .teardown = 1,
	};

	tests[i++] = (t_token_content_expansion_test){
		.input = param_make_token(VAR_TYPE, "$NON_EXISTENT", AS_LITERAL),
		.expected_output = param_make_token(WORD_TYPE, "", MAKE_COPY),
		.func = '$',
		.in_place_modification = true,
		.setup = 1, .teardown = 1,
	};

	return cr_make_param_array(t_token_content_expansion_test, tests, i);
}

void cleanup_params(struct criterion_test_params *ctp) {
	t_token_content_expansion_test *data = (t_token_content_expansion_test *)ctp;
	if (data->input && data->input->allocated)
		cr_free(data->input->content.expanded_word);
	if (data->input)
		cr_free(data->input);

	if (data->expected_output && data->expected_output->allocated)
		cr_free(data->expected_output->content.expanded_word);
	if (data->expected_output)
		cr_free(data->expected_output);
}

ParameterizedTest(t_token_content_expansion_test *testdata, expand_functions, all)
{
	// probably something with forking of multi threading
	// makes functions have different addresses here then when defined during
	// the setup of the parametized test
	static const f_func			f[256] = {
		['w'] = &expand_word_type,
		['$'] = &expand_var_type,
		['\"'] = &expand_dquote_type,
		['\''] = &expand_simple_quote_type,
		['~'] = &expand_tilde_type,
		['e'] = &expand_exitcode_type,
	};
	static const f_setup_teardown	s[100] = {
		[0] = NULL,
		[1] = setup_fake_env,
		[2] = setup_exitcode,
	};
	static const f_setup_teardown	t[100] = {
		[0] = NULL,
		[1] = teardown_fake_env,
		[2] = teardown_exitcode
	};
	t_response	response;
	t_token		*output;

	if (testdata->setup)
		s[testdata->setup]();

	response = f[testdata->func](testdata->input);
	cr_expect(response.status == testdata->expected_status,
		"Wrong response status, was %s, expected: %s",
		debug_status_code_from_enum(response.status), debug_status_code_from_enum(testdata->expected_status));

	if (response.status == testdata->expected_status)
	{
		output = response.data;


		// check token type
		cr_expect(output->type == testdata->expected_output->type,
			"Wrong token type, is %s, expected: %s",
			debug_token_type_from_enum(output->type), debug_token_type_from_enum(testdata->expected_output->type));

		// check token string content
		cr_expect_str_eq(output->content.expanded_word,
						testdata->expected_output->content.expanded_word,
			"Wrong text result, is [%.20s], expected: [%.20s] (truncated to 20 chars)",
			output->content.expanded_word, testdata->expected_output->content.expanded_word);

		// check token content length
		cr_expect(output->length == testdata->expected_output->length,
			"wrong token length, is %zu, expected: %zu",
			output->length, testdata->expected_output->length);


		if (testdata->in_place_modification)
			cr_expect(output == testdata->input,
				"token is not modified in place, but replaced by new token");
		else
			cr_expect(output != testdata->input,
				"token is modified in place, and not replaced by new token");
	}

	if (testdata->teardown)
		t[testdata->teardown]();
}
