/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/11 11:49:33 by vvissche      #+#    #+#                 */
/*   Updated: 2022/06/07 17:56:35 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include "libft.h"
#include "ft_printf.h"
#include "utils/minishell_colors.h"
#include "utils/exitcode.h"
#include "utils/interactive.h"

static bool	not_valid_numeric_exitcode(char *input)
{
	while (ft_isspace(*input))
		input++;
	if (*input == '-' || *input == '+')
		input++;
	while (ft_isdigit(*input))
		input++;
	if (*input == '\0')
		return (false);
	return (true);
}

static void	exit_numeric_arg_required(char *input)
{
	if (is_interactive())
		write(STDERR_FILENO, "exit\n", 5);
	ft_dprintf(STDERR_FILENO, "%s: exit: %s: numeric argument required\n",
		ERROR_EMOJI, input);
	exit(255);
}

static long long	get_exitcode_from_string(char *input)
{
	long long	exitcode;

	errno = 0;
	exitcode = ft_strtoll(input, NULL, 10);
	if (errno == ERANGE)
	{
		exit_numeric_arg_required(input);
	}
	return (exitcode);
}

int	builtin_exit(char *const *args)
{
	long long	exitcode;

	if (args[1] == NULL)
	{
		if (is_interactive())
			write(STDERR_FILENO, "exit\n", 5);
		exit((unsigned char)get_last_exit_code());
	}
	if (not_valid_numeric_exitcode(args[1]))
		exit_numeric_arg_required(args[1]);
	exitcode = get_exitcode_from_string(args[1]);
	if (args[2] != NULL)
	{
		if (is_interactive())
			write(STDERR_FILENO, "exit\n", 5);
		ft_dprintf(STDERR_FILENO, "%s: exit: too many arguments\n",
			ERROR_EMOJI);
		return (1);
	}
	if (is_interactive())
		write(STDERR_FILENO, "exit\n", 5);
	exit((unsigned char)exitcode);
	return (get_last_exit_code());
}
