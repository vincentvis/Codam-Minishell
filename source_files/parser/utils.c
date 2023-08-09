/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/07/30 12:13:21 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/07 18:18:18 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/token.h"
#include "parser/parser.h"
#include "parser/validation.h"
#include "utils/status.h"

t_response	parse_token_type(
		t_token **token,
		bool validator(t_token_type),
		bool required
	)
{
	t_ast_node	*node;

	if (!token
		|| !(*token)
		|| (validator != NULL
			&& validator((*token)->type) == false))
	{
		if (required)
		{
			return (make_response(P_BAD_INPUT, *token));
		}
		else
		{
			return (make_response(SUCCESS, NULL));
		}
	}
	node = ast_make_node_and_consume(token);
	if (!node)
		return (make_response(MALLOC_FAILURE, NULL));
	return (make_response(SUCCESS, node));
}
