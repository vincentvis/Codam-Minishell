/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unset.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/10 18:01:21 by vvissche      #+#    #+#                 */
/*   Updated: 2022/06/07 17:56:30 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "env/she_env.h"
#include "utils/status.h"
#include "ft_printf.h"
#include "utils/minishell_colors.h"
#include "builtins/builtins_internal.h"

static int	handle_sheenv_response(int status, char *arg)
{
	if (status == SHEENV_BAD_INPUT)
	{
		ft_dprintf(STDERR_FILENO, "%s: unset: `%s`: not a valid identifier\n",
			ERROR_EMOJI, arg);
		return (BUILTIN_GENERIC_FAILURE);
	}
	return (BUILTIN_SUCCESS);
}

int	builtin_unset(char *const *args)
{
	size_t		i;
	int			return_value;
	t_response	response;

	if (args[1] == NULL)
		return (BUILTIN_SUCCESS);
	return_value = 0;
	i = 1;
	while (args[i])
	{
		response = sheenv_unset(args[i]);
		if (response.status != SUCCESS)
		{
			return_value = handle_sheenv_response(response.status, args[i]);
		}
		i++;
	}
	return (return_value);
}
