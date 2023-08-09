/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe_sequence.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/07/29 17:17:11 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 18:18:08 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/token.h"
#include "parser/parser.h"
#include "parser/validation.h"
#include "utils/status.h"

__attribute__((always_inline))
static int	parse_command_fail(t_response response, t_token **token)
{
	return (response.status != SUCCESS
		|| response.data == NULL
		|| *token == NULL
		|| (*token)->type != PIPE_TYPE);
}

t_response	parse_pipe_fail(t_response response, void *to_free)
{
	free_tree_and_non_source_tokens(to_free, LEXER);
	return (response);
}

t_response	parse_pipe_sequence(t_token **token)
{
	t_response	response;
	t_response	pipe;

	response = parse_command(token);
	if (parse_command_fail(response, token))
	{
		if (response.status == SUCCESS && response.data == NULL)
			return (make_response(P_BAD_INPUT, *token));
		return (response);
	}
	pipe = parse_token_type(token, is_pipe, REQUIRED);
	if (pipe.status != SUCCESS)
		return (parse_pipe_fail(pipe, response.data));
	((t_ast_node *)pipe.data)->left = response.data;
	response = parse_pipe_sequence(token);
	if (response.status != SUCCESS)
		return (parse_pipe_fail(response, pipe.data));
	if (response.status == SUCCESS && response.data == NULL)
		return (parse_pipe_fail(make_response(P_BAD_INPUT, NULL), pipe.data));
	((t_ast_node *)pipe.data)->right = response.data;
	return (pipe);
}
