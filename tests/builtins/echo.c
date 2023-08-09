
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "builtins/builtins.h"
#include <stdio.h>
#include <unistd.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
	Echo displays a line of text
	by default, it displays it with a trailing newline
	if `-n` option is present, it does not output the trailing newline

	the return value of write needs to be checked
	and if that is -1 echo should return 1... (lol)

	if no arguments are supplied, a newline is printed

*/

Test(builtin_echo,
	no_args_prints_newline,
	.disabled = false)
{
	char *args[] = {
		"echo",
		NULL,
	};

	cr_redirect_stdout();

	int returnvalue = builtin_echo(args);

	cr_assert_stdout_eq_str("\n");
	cr_assert(returnvalue == 0);
}


Test(builtin_echo,
	only_dashn_should_print_nothing,
	.disabled = false)
{
	char *args[] = {
		"echo",
		"-n",
		NULL,
	};

	cr_redirect_stdout();

	int returnvalue = builtin_echo(args);
	// writing something to STDOUT because
	// criterion cannot handle it to be empty.......
	// but we can check if only our `!` is there
	write(STDOUT_FILENO, "!", 1);

	cr_assert_stdout_eq_str("!");
	cr_assert(returnvalue == 0);
}

Test(builtin_echo,
	multiple_args_separated_by_spaces,
	.disabled = false)
{
	char *args[] = {
		"echo",
		"string1",
		"string2",
		NULL,
	};

	cr_redirect_stdout();

	int returnvalue = builtin_echo(args);

	cr_assert_stdout_eq_str("string1 string2\n");
	cr_assert(returnvalue == 0);
}

Test(builtin_echo,
	multiple_args_separated_by_spaces_no_newline,
	.disabled = false)
{
	char *args[] = {
		"echo",
		"-n",
		"string1",
		"string2",
		NULL,
	};

	cr_redirect_stdout();

	int returnvalue = builtin_echo(args);

	cr_assert_stdout_eq_str("string1 string2");
	cr_assert(returnvalue == 0);
}

Test(builtin_echo,
	multiple_dashn_skipped,
	.disabled = false)
{
	char *args[] = {
		"echo",
		"-n",
		"-n",
		"-n",
		"string1",
		"string2",
		NULL,
	};

	cr_redirect_stdout();

	int returnvalue = builtin_echo(args);

	cr_assert_stdout_eq_str("string1 string2");
	cr_assert(returnvalue == 0);
}

Test(builtin_echo,
	odd_dasn_combos,
	.disabled = false)
{
	char *args[] = {
		"echo",
		"-n",
		"-nnnnnnnnnnn",
		"-nn",
		"string1",
		"string2",
		NULL,
	};

	cr_redirect_stdout();

	int returnvalue = builtin_echo(args);

	cr_assert_stdout_eq_str("string1 string2");
	cr_assert(returnvalue == 0);
}

Test(builtin_echo,
	first_option_starts_with_dashn_but_contains_more,
	.disabled = false)
{
	char *args[] = {
		"echo",
		"-no",
		"string1",
		"string2",
		NULL,
	};

	cr_redirect_stdout();

	int returnvalue = builtin_echo(args);

	cr_assert_stdout_eq_str("-no string1 string2\n");
	cr_assert(returnvalue == 0);
}

Test(builtin_echo,
	not_first_option_starts_with_dashn_but_contains_more,
	.disabled = false)
{
	char *args[] = {
		"echo",
		"-n",
		"-nx",
		"string1",
		"string2",
		NULL,
	};

	cr_redirect_stdout();

	int returnvalue = builtin_echo(args);

	cr_assert_stdout_eq_str("-nx string1 string2");
	cr_assert(returnvalue == 0);
}

extern int errno;

Test(builtin_echo,
	dup_stdout_to_file_without_write_perms,
	.disabled = false)
{
	char *args[] = {
		"echo",
		"test",
		NULL
	};

	int fd = open("tmpfile_dup_stdout_to_file_without_write_perms", O_CREAT | O_RDONLY);
	unlink("tmpfile_dup_stdout_to_file_without_write_perms");

	dup2(fd, STDOUT_FILENO);
	int returnvalue = builtin_echo(args);
	dup2(STDOUT_FILENO, fd);

	cr_assert(returnvalue == 1);
}
