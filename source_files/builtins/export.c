/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/10 18:01:21 by vvissche      #+#    #+#                 */
/*   Updated: 2022/06/07 17:56:32 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "env/she_env.h"
#include "ft_printf.h"
#include "utils/minishell_colors.h"

static void	handle_non_success_response(int status, char *arg)
{
	if (status == SHEENV_BAD_INPUT)
	{
		ft_dprintf(STDERR_FILENO,
			ERROR_EMOJI ": export: `%s': not a valid identifier\n",
			arg);
	}
	if (status == MALLOC_FAILURE)
	{
		ft_dprintf(STDERR_FILENO, ERROR_EMOJI ": export: allocation failure\n");
	}
}

int	builtin_export(char *const *args)
{
	size_t		i;
	int			return_value;
	t_response	response;

	if (args[1] == NULL)
		return (sheenv_print_export());
	return_value = 0;
	i = 1;
	while (args[i])
	{
		response = sheenv_store_from_input(args[i]);
		if (response.status != SUCCESS)
		{
			handle_non_success_response(response.status, args[i]);
			return_value = 1;
		}
		i++;
	}
	return (return_value);
}
