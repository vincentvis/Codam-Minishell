#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "builtins/builtins.h"
#include <limits.h>

#include <stdio.h>
#include "env/she_env.h"
#include "../generic_test_helpers.h"
#include "utils/readline.h"
#include "utils/minishell_colors.h"

#include <unistd.h>

Test(builtin_exit,
	call_with_two_args)
{
	char *args[] = {
		"exit",
		"4",
		"3",
		NULL
	};

	cr_redirect_stdout();
	cr_redirect_stderr();

	int return_value = builtin_exit(args);

	cr_assert(return_value == 1);

	write(STDOUT_FILENO, "!", 1);
	cr_assert_stdout_eq_str("!", "");

	char expected_stderr[1024];
	sprintf(expected_stderr, "exit\n%s: exit: too many arguments\n", ERROR_EMOJI);

	cr_assert_stderr_eq_str(expected_stderr, "");
}

// add: .exit_code = 255
Test(builtin_exit,
	call_with_numeric)
{
	char *args[] = {
		"exit",
		"5",
		NULL
	};


	cr_redirect_stdout();
	cr_redirect_stderr();

	pid_t p_id = fork();
	cr_assert(p_id >= 0);
	if (p_id == 0)
	{
		builtin_exit(args);
		exit(100);
	}

	if (p_id != 0)
	{
		int stat_loc;
		waitpid(p_id, &stat_loc, 0);

		cr_assert(WIFEXITED(stat_loc) == 1);
		cr_assert(WEXITSTATUS(stat_loc) == 5);

		write(STDOUT_FILENO, "!", 1);
		cr_assert_stdout_eq_str("!", "");
		cr_assert_stderr_eq_str("exit\n", "");
	}
}

Test(builtin_exit,
	call_with_non_numeric)
{
	char *args[] = {
		"exit",
		"a",
		NULL
	};

	cr_redirect_stderr();
	cr_redirect_stdout();

	pid_t p_id = fork();
	cr_assert(p_id >= 0);
	if (p_id == 0)
	{
		builtin_exit(args);
		exit(100);
	}

	if (p_id != 0)
	{
		int stat_loc;
		waitpid(p_id, &stat_loc, 0);

		cr_assert(WIFEXITED(stat_loc) == 1);
		cr_assert(WEXITSTATUS(stat_loc) == 255);

		char expected_stderr[1024];
		sprintf(expected_stderr, "exit\n%s: exit: %s: numeric argument required\n", ERROR_EMOJI, args[1]);

		cr_assert_stderr_eq_str(expected_stderr, "");

		write(STDOUT_FILENO, "!", 1);
		cr_assert_stdout_eq_str("!", "");
	}
}

Test(builtin_exit,
	call_with_non_numeric_2)
{
	char *args[] = {
		"exit",
		"a",
		"5",
		NULL
	};

	cr_redirect_stderr();
	cr_redirect_stdout();

	pid_t p_id = fork();
	cr_assert(p_id >= 0);
	if (p_id == 0)
	{
		builtin_exit(args);
		exit(100);
	}

	if (p_id != 0)
	{
		int stat_loc;
		waitpid(p_id, &stat_loc, 0);

		cr_assert(WIFEXITED(stat_loc) == 1);
		cr_assert(WEXITSTATUS(stat_loc) == 255, "stat %i", WEXITSTATUS(stat_loc));

		char expected_stderr[1024];
		sprintf(expected_stderr, "exit\n%s: exit: %s: numeric argument required\n", ERROR_EMOJI, args[1]);

		cr_assert_stderr_eq_str(expected_stderr, "");

		write(STDOUT_FILENO, "!", 1);
		cr_assert_stdout_eq_str("!", "");
	}
}

Test(builtin_exit,
	call_long_max)
{
	char *args[] = {
		"exit",
		"9223372036854775807",
		NULL
	};

	cr_redirect_stderr();
	cr_redirect_stdout();

	pid_t p_id = fork();
	cr_assert(p_id >= 0);
	if (p_id == 0)
	{
		builtin_exit(args);
		exit(100);
	}

	if (p_id != 0)
	{
		int stat_loc;
		waitpid(p_id, &stat_loc, 0);

		cr_assert(WIFEXITED(stat_loc) == 1);
		cr_assert(WEXITSTATUS(stat_loc) == ((unsigned char)9223372036854775807));

		write(STDOUT_FILENO, "!", 1);
		cr_assert_stdout_eq_str("!", "");
		cr_assert_stderr_eq_str("exit\n", "");
	}
}

Test(builtin_exit,
	call_long_max_plus_one)
{
	char *args[] = {
		"exit",
		"9223372036854775808",
		NULL
	};

	cr_redirect_stdout();
	cr_redirect_stderr();

	pid_t p_id = fork();
	cr_assert(p_id >= 0);
	if (p_id == 0)
	{
		builtin_exit(args);
		exit(100);
	}

	if (p_id != 0)
	{
		int stat_loc;
		waitpid(p_id, &stat_loc, 0);

		cr_assert(WIFEXITED(stat_loc) == 1);
		cr_assert(WEXITSTATUS(stat_loc) == ((unsigned char)9223372036854775807));

		char expected_stderr[1024];
		sprintf(expected_stderr, "exit\n%s: exit: %s: numeric argument required\n", ERROR_EMOJI, args[1]);

		cr_assert_stderr_eq_str(expected_stderr, "");
		write(STDOUT_FILENO, "!", 1);
		cr_assert_stdout_eq_str("!", "");
	}
}

Test(builtin_exit,
	first_arg_is_more_than_long_max_has_second_arg)
{
	char *args[] = {
		"exit",
		"9223372036854775808",
		"a",
		NULL
	};

	cr_redirect_stderr();
	cr_redirect_stdout();

	pid_t p_id = fork();
	cr_assert(p_id >= 0);
	if (p_id == 0)
	{
		builtin_exit(args);
		exit(100);
	}

	if (p_id != 0)
	{
		int stat_loc;
		waitpid(p_id, &stat_loc, 0);

		cr_expect(WIFEXITED(stat_loc) == 1);
		cr_expect(WEXITSTATUS(stat_loc) == 255);

		char expected_stderr[1024];
		sprintf(expected_stderr, "exit\n%s: exit: %s: numeric argument required\n", ERROR_EMOJI, args[1]);

		cr_assert_stderr_eq_str(expected_stderr, "");

		write(STDOUT_FILENO, "!", 1);
		cr_assert_stdout_eq_str("!", "");
	}
}
