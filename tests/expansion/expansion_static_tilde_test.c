
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>

#include "lexer/token.h"
#include "lexer/lexer_private.h"
#include "expansion/expansion.h"
#include "expansion/expand_functions.h"
#include "expansion/expand_validation.h"
#include "expansion_helpers.h"

#include "../../source_files/expansion/expansion.c"

typedef struct {
	t_token				*input;
	enum e_token_type	expected_result_type;
	size_t				length;
}	t_test_tilde;

/*
	If HOME is unset, the results are unspecified.
	https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_06_01
*/

Test(expansion, first_token_tilde_static)
{
	t_test_tilde tests[] = {
		{ .expected_result_type = WORD_TYPE, .input = make_token(WORD_TYPE, "a", false), .length = 1},
		{ .expected_result_type = WORD_TYPE, .input = make_token(WORD_TYPE, "~x", false), .length = 1},
		{ .expected_result_type = WORD_TYPE, .input = make_token(WORD_TYPE, "~~", false), .length = 1},
		{ .expected_result_type = WORD_TYPE, .input = make_token(WORD_TYPE, " ~", false), .length = 1},
		{ .expected_result_type = WORD_TYPE, .input = make_token(WORD_TYPE, "}/", false), .length = 1},
		{ .expected_result_type = WORD_TYPE, .input = make_token(WORD_TYPE, "}", false), .length = 1},

		{ .expected_result_type = TILDE_TYPE, .input = make_token(WORD_TYPE, "~", false), .length = 1},
		{ .expected_result_type = TILDE_TYPE, .input = make_token(WORD_TYPE, "~/", false), .length = 1},
		{ .expected_result_type = TILDE_TYPE, .input = make_token(WORD_TYPE, "~ ", false), .length = 1},

		{ .expected_result_type = WORD_TYPE, .input = make_token(WORD_TYPE, "~", false), .length = 2},
		{ .expected_result_type = WORD_TYPE, .input = make_token(WORD_TYPE, "~/", false), .length = 2},
		{ .expected_result_type = WORD_TYPE, .input = make_token(WORD_TYPE, "~ ", false), .length = 2},
		{ .input = NULL },
	};

	size_t	i = 0;

	while (tests[i].input)
	{
		// make length customisable
		tests[i].input->length = tests[i].length;
		// trigger the function on the token:
		is_first_token_tilde(tests[i].input);
		// test if type is not changed!
		cr_expect(tests[i].input->type == tests[i].expected_result_type);
		i++;
	}
}
