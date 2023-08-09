/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   status.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/07/15 17:00:03 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/08 09:31:06 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include "ft_printf.h"
#include "utils/status.h"
#include "lexer/token.h"
#include "utils/status.h"
#include "utils/minishell_colors.h"
#include "utils/readline.h"

static const char	*get_error_msg(const int status_code)
{
	static const char	*error_msg[STATUS_CODE_MAX] = {
	[BAD_INPUT] = "syntax error: general",
	[MALLOC_FAILURE] = NULL,
	[SHEENV_BAD_INPUT] = "bad input for env",
	[SHEENV_DOUBLE_INIT] = "tying to initialize twice",
	[SHEENV_UNINITIALIZED] = "trying to access uninitialized env",
	[SHEENV_KEY_NOT_FOUND] = "key not found in env",
	[L_BAD_INPUT] = "syntax error: missing closing quote",
	[P_BAD_INPUT] = "syntax error near unexpected token",
	[E_FILE_FAILURE] = NULL,
	[E_FD_DUPLICATION_FAILURE] = NULL,
	[E_PIPE_FAILURE] = NULL,
	[E_FORK_FAILURE] = NULL,
	[E_CLOSE_FAILURE] = NULL,
	[E_COMMAND_NOT_FOUND] = "command not found",
	[E_AMBIGUOUS_REDIRECT] = "ambiguous redirect",
	[E_HERE_FILE_FAILURE] = NULL,
	[E_HERE_FAILURE] = NULL,
	[WRITE_FAILURE] = NULL,
	};

	return (error_msg[status_code]);
}

static t_error_handler	get_error_handler(int status)
{
	static const t_error_handler	error_handlers[STATUS_CODE_MAX] = {
	[BAD_INPUT] = log_error_with_token,
	[MALLOC_FAILURE] = log_perror,
	[SHEENV_BAD_INPUT] = log_error,
	[SHEENV_DOUBLE_INIT] = log_error,
	[SHEENV_UNINITIALIZED] = log_error,
	[SHEENV_KEY_NOT_FOUND] = log_error,
	[L_BAD_INPUT] = log_error,
	[P_BAD_INPUT] = log_error_with_token,
	[E_FILE_FAILURE] = log_perror,
	[E_FD_DUPLICATION_FAILURE] = log_perror,
	[E_PIPE_FAILURE] = log_perror,
	[E_FORK_FAILURE] = log_perror,
	[E_CLOSE_FAILURE] = log_perror,
	[E_COMMAND_NOT_FOUND] = log_error,
	[E_AMBIGUOUS_REDIRECT] = log_error,
	[E_HERE_FILE_FAILURE] = log_perror,
	[E_HERE_FAILURE] = NULL,
	[WRITE_FAILURE] = log_perror,
	};

	return (error_handlers[status]);
}

int	manage_error(const int status_code, void *reference)
{
	const char				*error_msg = get_error_msg(status_code);
	const t_error_handler	handler = get_error_handler(status_code);

	if (handler)
		handler(status_code, reference, error_msg);
	return (status_code);
}

t_response	make_response(int status, void *data)
{
	t_response	response;

	response.data = data;
	response.status = status;
	return (response);
}
