
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

/*
	https://www.man7.org/linux/man-pages/man1/cd.1p.html

	syntax:
	cd  [dir]

	Change the current directory to DIR.
	The default DIR is the value of the HOME shell variable.
	Any  additional arguments following dir are ignored.

	The variable CDPATH defines the search path for the directory containing
	DIR.  Alternative directory names in CDPATH are separated by a colon (:).
	A null directory name is the same as the current directory.  If DIR begins
	with a slash (/), then CDPATH is not used.

	If the directory is not found, and the shell option `cdable_vars' is set,
	the word is assumed to be  a variable name.  If that variable has a value,
	its value is used for DIR.

	`..' is processed by removing the immediately previous pathname component
	back to a slash or the beginning of DIR.

	Exit Status:
	Returns 0 if the directory is changed, and if $PWD is set successfully when
	-P is used; non-zero otherwise.


	https://www.man7.org/linux/man-pages/man1/cd.1p.html
	CD shall be executed following these steps:

	1.	if no `dir` is given and the HOME env variable is empty or undefined
		the default behaviour is implementation defined and no further steps
		shall be taken
		>> bash does:
			bash: cd: HOME not set
		so let's do that..

	2.	if no `dir` is given and HOME is set and not empty cd shall behave as if
		`dir` was set to the contents of HOME

	3.	if `dir` starts with a <slash> character, set curpath variable to `dir`
		and proceed to step 7

	4.	if the first component of `dir` is dot or dot-dot proceed to step 6

	5.	Starting with the first pathname in the <colon>-separated pathnames of
		CDPATH (see the ENVIRONMENT VARIABLES section) if the pathname is
		non-null, test if the concatenation of that pathname, a <slash>
		character if that pathname did not end with a <slash> character,
		and the directory operand names a directory. If the pathname is null,
		test if the concatenation of dot, a <slash> character, and the operand
		names a directory. In either case, if the resulting string names an
		existing directory, set curpath to that string and proceed to step 7.
		Otherwise, repeat this step with the next pathname in CDPATH until all
		pathnames have been tested.

	6.	Set curpath to the directory operand.

	7.	If the -P option is in effect, proceed to step 10. If curpath
		does not begin with a <slash> character, set curpath to the
		string formed by the concatenation of the value of PWD, a
		<slash> character if the value of PWD did not end with a
		<slash> character, and curpath.

	8.	The curpath value shall then be converted to canonical form
		as follows, considering each component from beginning to end,
		in sequence:

		a.	Dot components and any <slash> characters that separate
			them from the next component shall be deleted.

		b.	For each dot-dot component, if there is a preceding
			component and it is neither root nor dot-dot, then:

			i.	If the preceding component does not refer (in the
				context of pathname resolution with symbolic links
				followed) to a directory, then the cd utility shall
				display an appropriate error message and no further
				steps shall be taken.

			ii.	The preceding component, all <slash> characters
				separating the preceding component from dot-dot,
				dot-dot, and all <slash> characters separating dot-
				dot from the following component (if any) shall be
				deleted.

		c.	An implementation may further simplify curpath by
			removing any trailing <slash> characters that are not
			also leading <slash> characters, replacing multiple non-
			leading consecutive <slash> characters with a single
			<slash>, and replacing three or more leading <slash>
			characters with a single <slash>.  If, as a result of
			this canonicalization, the curpath variable is null, no
			further steps shall be taken.

	9.	If curpath is longer than {PATH_MAX} bytes (including the
		terminating null) and the directory operand was not longer
		than {PATH_MAX} bytes (including the terminating null), then
		curpath shall be converted from an absolute pathname to an
		equivalent relative pathname if possible. This conversion
		shall always be considered possible if the value of PWD, with
		a trailing <slash> added if it does not already have one, is
		an initial substring of curpath.  Whether or not it is
		considered possible under other circumstances is unspecified.
		Implementations may also apply this conversion if curpath is
		not longer than {PATH_MAX} bytes or the directory operand was
		longer than {PATH_MAX} bytes.

   10.	The cd utility shall then perform actions equivalent to the
		chdir() function called with curpath as the path argument. If
		these actions fail for any reason, the cd utility shall
		display an appropriate error message and the remainder of
		this step shall not be executed. If the -P option is not in
		effect, the PWD environment variable shall be set to the
		value that curpath had on entry to step 9 (i.e., before
		conversion to a relative pathname). If the -P option is in
		effect, the PWD environment variable shall be set to the
		string that would be output by pwd -P.  If there is
		insufficient permission on the new directory, or on any
		parent of that directory, to determine the current working
		directory, the value of the PWD environment variable is
		unspecified.

		If, during the execution of the above steps, the PWD environment
		variable is set, the OLDPWD environment variable shall also be
		set to the value of the old working directory (that is the
		current working directory immediately prior to the call to cd).

NOTES
- let's not do CDPATH, since it is not specified anywhere and it is more work

	CD shall take these steps for our minishel builtin version:

	1.	if no `dir` is given and the HOME env variable is empty or undefined
		our shell shall print to stderr:
			`bash: cd: HOME not set`

	2.	if no `dir` is given and the HOME env variable is set and not empty
		cd shall behave as if `dir` was set with the contents of HOME

	3.	if `dir` starts with a <slash> character,
		set `curpath` variable to `dir` and proceed to step 7

	4.	if the first component of `dir` is <dot> or <dot-dot> proceed to step 6

	5.	STEP SKIPPED FROM ORIGINAL

	6.	set `curpath` to `dir`

	7.	if `curpath` does not start with <slash> character, set `curpath` to
		the string formed by concatenation of
		- the value of PWD,
		- a <slash> character if the value of PWD did not end with a <slash>
		- the `curpath`

	8.	convert `curpath` to canonical form as follows, considering each
		component, from beginning to end, in sequence:

		a.	<dot> components and any <slash> characters that separate them from
			the next component will be deleted

		b.	For each <dot-dot> component, if there is a preceding component and
			it is neither root nor <dot-dot>, then:

				i.	if the preceding component does not refer (in the context
					of pathname resolution with symbolic links followed) to a
					directory, then the cd utility shall display anappropiate
					error message and no further steps shall be taken.

				ii.	the preceding component, all ,slash> characters separating
					the preceding component from <dot-dot>, <dot-dot>, and all
					<slash> characters separating <dot-dot> from the beginning
					component (if any) shall be deleted

		c.	An implementation may further simplify `curpath` by removing any
			trailing <slash> characters that are not also leading <slash>
			characters, replacing mulitple non-leading <slash> characters with
			a single <slash>, and preplacing three or more leasing <slash>
			characters with a single <slash>. If, as a result of this
			canonicalization, the curpath vairable is null, no further steps
			shall be taken

	9.	this is a section about {PATH_MAX} length amount of bytes for the
		`curpath` stuff, but we are skipping that because fuck that

	10.	the cd utility shall then perform actions equivalent to chdir() function
		called with `curpath` as the path argument.
		If these actions fail for any reason, the cd utility shall display an
		appropiate error message and the remainder of this step shall not be
		executed.
		The PWD env variabele shall be set to the value that `curpath` had on
		entry of step 9 (i.e. before conversion to a relative pathname).
		If there is insufficient permission ont he new directory, or on any
		parent of that direcotry, to determine the current working directory,
		the value of the PWD env variable is unspecified

		If, during the execution of the above steps, the PWD variable is set,
		the OLDPWD env variable shall also be set to the value of the old
		working directory (that is the current working directory immediately
		prior to the call of cd)

*/

char	*args_without_dir[] = {
	"echo",
	NULL
};

// (1)
Test(builtin_cd,
	no_dir_no_home_prints_stderr,
	.init = setup_empty_env)
{
	cr_redirect_stderr();
	cr_redirect_stdout();

	// call the function
	int return_value = builtin_cd(args_without_dir);

	// make sure stdout is populated with something
	write(STDOUT_FILENO, "!", 1);
	// make sure nothing is written to stdout
	cr_assert_stdout_eq_str("!", "");

	cr_assert(return_value == BUILTIN_GENERIC_FAILURE);

	char expected_stderr[1024];
	sprintf(expected_stderr, "%s: %s\n", ERROR_EMOJI, "cd: HOME not set");

	cr_assert_stderr_eq_str(expected_stderr, "");

	sheenv_destroy();
}

// (1)
Test(builtin_cd,
	no_dir_empty_home_prints_stderr,
	.init = setup_empty_env)
{
	sheenv_store_from_key("HOME");

	cr_redirect_stderr();
	cr_redirect_stdout();

	// call the function
	int return_value = builtin_cd(args_without_dir);

	// make sure stdout is populated with something
	write(STDOUT_FILENO, "!", 1);
	// make sure nothing is written to stdout
	cr_assert_stdout_eq_str("!", "");

	cr_assert(return_value == BUILTIN_GENERIC_FAILURE);

	char expected_stderr[1024];
	sprintf(expected_stderr, "%s: %s\n", ERROR_EMOJI, "cd: HOME not set");

	cr_assert_stderr_eq_str(expected_stderr, "");

	sheenv_destroy();
}

Test(builtin_cd,
	no_dir_goes_to_home_variable,
	.init = setup_empty_env,
	.disabled = false)
{
	// picking something that always exists I guess?
	sheenv_store_from_key_value("HOME", "/Users");
	sheenv_store_from_key_value("PWD", "/Library/Widgets");
	sheenv_store_from_key_value("OLDPWD", "/Library/Desktop Pictures");

	char *home_original = strdup(sheenv_get_value_by_key("HOME").data);
	char *pwd_original = strdup(sheenv_get_value_by_key("PWD").data);
	char *oldpwd_original = strdup(sheenv_get_value_by_key("OLDPWD").data);

	int return_value = builtin_cd(args_without_dir);
	cr_assert(return_value == BUILTIN_SUCCESS);

	char *home_after = sheenv_get_value_by_key("HOME").data;
	char *pwd_after = sheenv_get_value_by_key("PWD").data;
	char *oldpwd_after = sheenv_get_value_by_key("OLDPWD").data;

	cr_assert_str_eq(home_original, home_after, "HOME variable should stay the same");
	cr_assert_str_eq(pwd_original, oldpwd_after, "OLDPWD should be set to previous PWD value");
	cr_assert_str_eq(home_original, pwd_after, "PWD should be set to value of HOME");
	cr_assert_str_neq(oldpwd_original, oldpwd_after, "OLDPWD should be updated");
}

Test(builtin_cd,
	unset_pwd_sets_empty_oldpwd,
	.init = setup_empty_env)
{
	sheenv_store_from_key_value("HOME", "/Users");
	sheenv_store_from_key_value("OLDPWD", "/Library/Desktop Pictures");

	char *args[] = {
		"cd",
		"/Library"
	};

	int return_value = builtin_cd(args);

	cr_assert(return_value == BUILTIN_SUCCESS);

	char *pwd_after = sheenv_get_value_by_key("PWD").data;
	char *oldpwd_after = sheenv_get_value_by_key("OLDPWD").data;

	cr_assert_str_eq(pwd_after, args[1]);
	cr_assert_str_eq(oldpwd_after, "");
}

Test(builtin_cd,
	with_existing_dir,
	.init = setup_empty_env)
{
	const char	*pwd = "/Library/Widgets";
	sheenv_store_from_key_value("HOME", "/Users");
	sheenv_store_from_key_value("PWD", pwd);
	sheenv_store_from_key("OLDPWD");

	char *args[] = {
		"cd",
		"/Library/Application Support/Apple",
		NULL
	};

	int return_value = builtin_cd(args);
	cr_assert(return_value == BUILTIN_SUCCESS);

	char *pwd_after = sheenv_get_value_by_key("PWD").data;
	char *oldpwd_after = sheenv_get_value_by_key("OLDPWD").data;

	cr_assert_str_eq(pwd_after, args[1]);
	cr_assert_str_eq(oldpwd_after, pwd);
}

Test(builtin_cd,
	with_dot_dot,
	.init = setup_empty_env)
{
	const char	*pwd = "/Library/Logs";
	sheenv_store_from_key_value("HOME", "/Users");
	sheenv_store_from_key_value("PWD", pwd);
	sheenv_store_from_key("OLDPWD");
	// make sure we are in PWD
	chdir(pwd);

	char *args[] = {
		"cd",
		"/Library/Application Support/../../Library/Widgets",
		NULL
	};

	int return_value = builtin_cd(args);
	cr_assert(return_value == BUILTIN_SUCCESS);

	char *pwd_after = sheenv_get_value_by_key("PWD").data;
	char *oldpwd_after = sheenv_get_value_by_key("OLDPWD").data;

	cr_assert_str_eq(pwd_after, "/Library/Widgets");
	cr_assert_str_eq(oldpwd_after, pwd);
}

Test(builtin_cd,
	with_dot_eg_relative_path,
	.init = setup_empty_env)
{
	const char	*pwd = "/Library";
	sheenv_store_from_key_value("HOME", "/Users");
	sheenv_store_from_key_value("PWD", pwd);
	sheenv_store_from_key("OLDPWD");
	// make sure we are in PWD
	chdir(pwd);

	char *args[] = {
		"cd",
		"./Logs",
		NULL
	};

	int return_value = builtin_cd(args);
	cr_assert(return_value == BUILTIN_SUCCESS);

	char *pwd_after = sheenv_get_value_by_key("PWD").data;
	char *oldpwd_after = sheenv_get_value_by_key("OLDPWD").data;

	cr_assert_str_eq(pwd_after, "/Library/Logs");
	cr_assert_str_eq(oldpwd_after, pwd);
}

Test(builtin_cd,
	with_relative_path,
	.init = setup_empty_env)
{
	const char	*pwd = "/Library";
	sheenv_store_from_key_value("HOME", "/Users");
	sheenv_store_from_key_value("PWD", pwd);
	sheenv_store_from_key("OLDPWD");
	// make sure we are in PWD
	chdir(pwd);

	char *args[] = {
		"cd",
		"Logs",
		NULL
	};

	int return_value = builtin_cd(args);
	cr_assert(return_value == BUILTIN_SUCCESS);

	char *pwd_after = sheenv_get_value_by_key("PWD").data;
	char *oldpwd_after = sheenv_get_value_by_key("OLDPWD").data;

	cr_assert_str_eq(pwd_after, "/Library/Logs");
	cr_assert_str_eq(oldpwd_after, pwd);
}

Test(builtin_cd,
	to_non_existing_dir,
	.init = setup_empty_env)
{
	const char	*pwd = "/Library/Logs";
	sheenv_store_from_key_value("HOME", "/Users");
	sheenv_store_from_key_value("PWD", pwd);
	sheenv_store_from_key_value("OLDPWD", "/should_not_update_this");

	char *args[] = {
		"cd",
		"/NON_EXISTENT_SOMEHTING_SOMEHTING/bla",
		NULL
	};

	int return_value = builtin_cd(args);
	cr_assert(return_value == BUILTIN_GENERIC_FAILURE);

	char *pwd_after = sheenv_get_value_by_key("PWD").data;
	char *oldpwd_after = sheenv_get_value_by_key("OLDPWD").data;

	cr_assert_str_eq(pwd_after, pwd);
	cr_assert_str_eq(oldpwd_after, "/should_not_update_this");
}
