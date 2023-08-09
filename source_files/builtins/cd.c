/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/10 15:39:03 by vvissche      #+#    #+#                 */
/*   Updated: 2022/06/07 17:54:46 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <unistd.h>
#include "libft.h"
#include "ft_printf.h"
#include "env/she_env.h"
#include "utils/exitcode.h"
#include "utils/minishell_colors.h"
#include "builtins/builtins_internal.h"

static int	cd_error(char *msg)
{
	ft_dprintf(STDERR_FILENO, ERROR_EMOJI ": cd: %s\n", msg);
	return (BUILTIN_GENERIC_FAILURE);
}

static int	change_dir(const char *dir)
{
	const t_response	get_pwd = sheenv_get_value_by_key("PWD");
	char				*pwd;
	t_response			response;
	char				*buf;
	int					return_value;

	return_value = chdir(dir);
	if (return_value == -1 && log_perror(store_exit_code(1), "cd", dir))
		return (BUILTIN_GENERIC_FAILURE);
	if (get_pwd.status == SUCCESS)
		pwd = ft_strdup(get_pwd.data);
	else
		pwd = ft_strdup("");
	if (pwd == NULL)
		return (BUILTIN_MALLOC_FAILURE);
	buf = getcwd(NULL, 0);
	if (buf)
	{
		response = sheenv_store_from_key_value("PWD", buf);
		free(buf);
		if (response.status == SUCCESS)
			sheenv_store_from_key_value("OLDPWD", pwd);
	}
	free(pwd);
	return (BUILTIN_SUCCESS);
}

static int	cd_without_args(void)
{
	const t_response	get_home = sheenv_get_struct_by_key("HOME");
	const t_env_var		*home_struct = get_home.data;

	if (get_home.status != SUCCESS
		|| home_struct->data[home_struct->key_length] == '\0')
		return (cd_error("HOME not set"));
	return (change_dir(&(home_struct->data[home_struct->key_length + 1])));
}

int	builtin_cd(char *const *args)
{
	if (args[1] == NULL)
		return (cd_without_args());
	return (change_dir(args[1]));
}
