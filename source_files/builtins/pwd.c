/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/10 17:37:08 by vvissche      #+#    #+#                 */
/*   Updated: 2022/05/30 13:25:35 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "ft_printf.h"
#include "env/she_env.h"
#include "builtins/builtins_internal.h"

int	print_pwd_from_env(void)
{
	const t_response	get_pwd = sheenv_get_value_by_key("PWD");

	if (get_pwd.status != SUCCESS)
		return (BUILTIN_GENERIC_FAILURE);
	if (ft_dprintf(STDOUT_FILENO, "%s\n", get_pwd.data) == -1)
	{
		perror("pwd");
		return (BUILTIN_GENERIC_FAILURE);
	}
	return (BUILTIN_SUCCESS);
}

int	builtin_pwd(char *const *args)
{
	char				*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (print_pwd_from_env());
	if (ft_dprintf(STDOUT_FILENO, "%s\n", pwd) == -1)
	{
		perror("pwd");
		return (BUILTIN_GENERIC_FAILURE);
	}
	free(pwd);
	(void)args;
	return (BUILTIN_SUCCESS);
}
