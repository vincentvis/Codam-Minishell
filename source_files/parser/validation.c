/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   validation.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/07/23 10:31:07 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/07 17:48:38 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "lexer/token.h"

bool	is_command_suffix(t_token_type type)
{
	if (type == WORD_TYPE || type > _REDIRECTION || type == IO_NUMBER_TYPE)
		return (true);
	else
		return (false);
}

bool	is_redirection(t_token_type type)
{
	return (type > _REDIRECTION);
}

bool	is_io_number(t_token_type type)
{
	return (type == IO_NUMBER_TYPE);
}

bool	is_word(t_token_type type)
{
	return (type == WORD_TYPE);
}

bool	is_pipe(t_token_type type)
{
	return (type == PIPE_TYPE);
}
