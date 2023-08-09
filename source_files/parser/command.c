/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   command.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/07/29 23:32:19 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/07 18:17:41 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/token.h"
#include "parser/parser.h"
#include "parser/validation.h"
#include "utils/status.h"

t_response	parse_command(t_token **token)
{
	const t_response	cmd_prefix = parse_command_prefix(token);
	t_response			cmd_name;
	t_response			cmd_suffix;

	if (cmd_prefix.status != SUCCESS)
		return (cmd_prefix);
	cmd_name = parse_command_name(token);
	if (cmd_name.status == SUCCESS && cmd_name.data == NULL)
		return (cmd_prefix);
	if (cmd_name.status != SUCCESS)
	{
		free_tree_and_non_source_tokens(cmd_prefix.data, LEXER);
		return (cmd_name);
	}
	cmd_suffix = parse_command_suffix(token);
	if (cmd_suffix.status != SUCCESS)
	{
		free_tree_and_non_source_tokens(cmd_prefix.data, LEXER);
		free_tree_and_non_source_tokens(cmd_name.data, LEXER);
		return (cmd_suffix);
	}
	((t_ast_node *)cmd_name.data)->left = cmd_prefix.data;
	((t_ast_node *)cmd_name.data)->right = cmd_suffix.data;
	return (cmd_name);
}

t_response	parse_command_prefix(t_token **token)
{
	const t_response	cmd_prefix = parse_io_redirect(token);
	t_response			right;

	if (cmd_prefix.status == SUCCESS && cmd_prefix.data != NULL)
	{
		right = parse_command_prefix(token);
		if (right.status != SUCCESS)
		{
			free_tree_and_non_source_tokens(cmd_prefix.data, LEXER);
			return (right);
		}
		((t_ast_node *)cmd_prefix.data)->right = right.data;
	}
	return (cmd_prefix);
}

t_response	parse_command_name(t_token **token)
{
	return (parse_token_type(token, is_word, OPTIONAL));
}

t_response	parse_command_suffix(t_token **token)
{
	t_response	cmd_suffix;
	t_response	right;

	if (*token == NULL || is_command_suffix((*token)->type) == false)
		return (make_response(SUCCESS, NULL));
	if (is_word((*token)->type))
		cmd_suffix = parse_token_type(token, is_word, REQUIRED);
	else
		cmd_suffix = parse_io_redirect(token);
	if (cmd_suffix.status != SUCCESS || cmd_suffix.data == NULL)
		return (cmd_suffix);
	right = parse_command_suffix(token);
	if (right.status != SUCCESS)
	{
		free_tree_and_non_source_tokens(cmd_suffix.data, LEXER);
		return (right);
	}
	((t_ast_node *)cmd_suffix.data)->right = right.data;
	return (cmd_suffix);
}
