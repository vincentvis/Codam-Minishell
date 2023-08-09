/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/04 13:27:16 by vvissche      #+#    #+#                 */
/*   Updated: 2022/06/07 17:54:44 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdbool.h>
#include "libft.h"
#include "builtins/builtins_internal.h"

static bool	is_valid_argument_for_builtin(char *string)
{
	if (*string != '-')
		return (false);
	string++;
	while (*string == 'n')
		string++;
	if (*string == '\0')
		return (true);
	return (false);
}

static bool	check_options(char *const *args, int *first_arg_index)
{
	bool	has_trailing_newline;

	has_trailing_newline = true;
	while (args && args[*first_arg_index]
		&& is_valid_argument_for_builtin(args[*first_arg_index]))
	{
		has_trailing_newline = false;
		(*first_arg_index)++;
	}
	return (has_trailing_newline);
}

static int	print_arguments(char *const *args, int i, bool add_newline)
{
	while (args[i])
	{
		if (write(STDOUT_FILENO, args[i], ft_strlen(args[i])) == -1)
			return (BUILTIN_WRITE_FAILURE);
		if (args[i + 1] && write(STDOUT_FILENO, " ", 1) == -1)
			return (BUILTIN_WRITE_FAILURE);
		i++;
	}
	if (add_newline && write(STDOUT_FILENO, "\n", 1) == -1)
		return (BUILTIN_WRITE_FAILURE);
	return (BUILTIN_SUCCESS);
}

int	builtin_echo(char *const *args)
{
	bool	add_newline;
	int		first_arg_index;

	if (args[1])
	{
		first_arg_index = 1;
		add_newline = check_options(args, &first_arg_index);
		return (print_arguments(args, first_arg_index, add_newline));
	}
	if (write(STDOUT_FILENO, "\n", 1) == -1)
		return (BUILTIN_WRITE_FAILURE);
	return (BUILTIN_SUCCESS);
}
