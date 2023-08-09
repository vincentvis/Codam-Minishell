/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_handlers.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/07 17:28:52 by vvissche      #+#    #+#                 */
/*   Updated: 2022/06/08 09:50:40 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "libft.h"
#include "ft_printf.h"
#include "utils/status.h"
#include "lexer/token.h"
#include "lexer/validation.h"
#include "utils/minishell_colors.h"

int	log_perror(
		const enum e_status_code status_code,
		void *reference,
		const char *msg
	)
{
	char	*full;

	full = NULL;
	if (reference && msg)
		ft_asprintf(&full, ERROR_EMOJI": %s: %s", reference, msg);
	else if (reference)
		ft_asprintf(&full, ERROR_EMOJI": %s", reference);
	else if (msg)
		ft_asprintf(&full, ERROR_EMOJI": %s", msg);
	else
		ft_asprintf(&full, ERROR_EMOJI);
	if (full == NULL)
	{
		perror("Malloc failed on generating prefix: ");
		return (status_code);
	}
	perror(full);
	free(full);
	return (status_code);
	(void)msg;
}

int	log_ambiguous_redirect_error(
		const enum e_status_code status_code,
		void *reference,
		const char *msg
	)
{
	char	*token;
	int		length;

	token = reference;
	while ((isleftchevron(*token) || isrightchevron(*token) || *token == ' ')
		&& *token)
		token++;
	length = 0;
	if (isdollarsign(token[length]))
		length++;
	if (ft_isdigit(token[length]))
	{
		ft_dprintf(STDERR_FILENO, ERROR_EMOJI ": %.*s: %s\n", 2, token, msg);
		return (status_code);
	}
	if (isvarstart(token[length]))
		length++;
	while (isinsidevar(token[length]) || isdollarsign(token[length]))
		length++;
	ft_dprintf(STDERR_FILENO, ERROR_EMOJI ": %.*s: %s\n", length, token, msg);
	return (status_code);
}

int	log_error(
		const enum e_status_code status_code,
		void *reference,
		const char *msg
	)
{
	if (msg)
		ft_dprintf(STDERR_FILENO, ERROR_EMOJI ": %s\n", msg);
	return (status_code);
	(void)reference;
}

int	log_error_with_token(
		const enum e_status_code status_code,
		void *reference,
		const char *msg
	)
{
	const t_token	*token = reference;

	if (token)
	{
		if (token->allocated)
		{
			ft_dprintf(STDERR_FILENO, ERROR_EMOJI ": %s `%s'\n", msg,
				token->content.expanded_word);
		}
		else
		{
			ft_dprintf(STDERR_FILENO, ERROR_EMOJI ": %s `%.*s'\n", msg,
				(int)token->length, token->content.raw_word);
		}
	}
	else
	{
		ft_dprintf(STDERR_FILENO, ERROR_EMOJI ": %s `newline'\n", msg);
	}
	return (status_code);
}
