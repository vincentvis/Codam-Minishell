
#ifndef SHE_ENV_TESTING_H
# define SHE_ENV_TESTING_H

extern const char **environ;

const char	*fake_environ[] = {
	[0] = "FIRST_VAR=Value with spaces",
	[1] = "__A=double underscore",
	[2] = "camelCaseVarName=testing",
	[3] = "ABC=DEF",
	[4] = NULL
};
#define NUM_ELEMS_IN_FAKE_ENVIRON 4

#define NUM_KEYS_ADDED_BY_DEFAULT 2
#define NUM_VALUES_ADDED_BY_DEFAULT 1

#define IS_DISABLED true
#define NOT_DISABLED false

void	cleanup_after(void)
{
	sheenv_destroy();
}

// works but only if we do not need the init variable
// since this var is not exposed to the rest of the function
void	setup_fake_env(void)
{
	t_response		init = sheenv_init(fake_environ);
	cr_assert(init.status == SUCCESS);
	cr_assert_not_null(init.data);
}

#endif
