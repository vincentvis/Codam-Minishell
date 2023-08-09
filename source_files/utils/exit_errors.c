/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit_errors.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/26 18:23:01 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 21:41:04 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "utils/minishell_colors.h"
#include "libft.h"
#include "ft_printf.h"
#include "utils/status.h"

void	error_and_exit(const char *prefix, const char *msg, int errorcode)
{
	if (prefix)
		ft_dprintf(STDERR_FILENO, ERROR_EMOJI ": %s: %s\n", prefix, msg);
	else
		ft_dprintf(STDERR_FILENO, ERROR_EMOJI ": %s\n", msg);
	exit(errorcode);
}

void	perror_and_exit(const char *prefix)
{
	const char	*full = ft_strjoin(ERROR_EMOJI ": ", prefix);

	perror(full);
	if (errno == ENOENT)
		exit(EXIT_CODE_NOT_FOUND);
	if (errno == EACCES)
		exit(EXIT_CODE_CANNOT_EXECUTE);
	exit(1);
}
