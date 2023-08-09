#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../include/utils/status.h"
#include "../include/utils/minishell_colors.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

Test(status, simple_success_response)
{
	t_response response = make_response(SUCCESS, NULL);

	cr_expect_eq(response.status, SUCCESS);
	cr_expect_null(response.data);
}

Test(status, simple_bad_input)
{
	t_response response = make_response(BAD_INPUT, NULL);

	cr_expect_eq(response.status, BAD_INPUT);
	cr_expect_null(response.data);
}

Test(status, simple_malloc_failure)
{
	t_response response = make_response(MALLOC_FAILURE, NULL);

	cr_expect_eq(response.status, MALLOC_FAILURE);
	cr_expect_null(response.data);
}

Test(status, simple_not_in_enum)
{
	t_response response = make_response(256, NULL);

	int state = response.status;
	cr_expect(state == 256);
	cr_expect_null(response.data);
}

Test(status, response_with_pointer)
{
	t_response data = {
		.data = (void*)256,
		.status = SUCCESS
	};
	t_response response = make_response(SUCCESS, &data);

	cr_expect_eq(response.status, SUCCESS);
	cr_expect_eq(response.data, &data);
	cr_expect_eq(((t_response*)(response.data))->data, data.data);
	cr_expect_eq(((t_response*)(response.data))->status, data.status);
}

Test(log_error, error_loging_1)
{
	int ret;
	// put in a define so we can make use of preprocessor string joining for adding newline
	#define ERR_MSG_1 "test message written to a file, testing if it is written with newline or not"

	cr_redirect_stderr();
	ret = log_error(25, NULL, ERR_MSG_1);
	// check stderr
	cr_assert_stderr_eq_str(ERROR_EMOJI ": " ERR_MSG_1 "\n", "");
	// check return value
	cr_expect_eq(ret, 25);

	#undef ERR_MSG_1
}
