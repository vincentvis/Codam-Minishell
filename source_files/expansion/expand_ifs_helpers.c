/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_ifs_helpers.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/08 16:38:41 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/13 09:43:12 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "expansion/expand_functions.h"
#include "lexer/token.h"

t_pos	make_position(t_token *token, char *line)
{
	t_pos	position;

	position.current_token = token;
	position.line = line;
	return (position);
}

bool	has_quotes(t_token *token)
{
	while (token)
	{
		if (token->type == QUOTE_TYPE || token->type == DQUOTE_TYPE)
			return (true);
		token = token->next;
	}
	return (false);
}
