/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   io_redirect.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/07/29 18:11:51 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/07 18:17:52 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/token.h"
#include "parser/parser.h"
#include "parser/validation.h"
#include "utils/status.h"

static bool	is_word_or_here_eof(t_token_type type)
{
	return (is_word(type)
		|| type == HERE_EOF_TYPE
		|| type == HERE_EOF_EXP_TYPE);
}

static t_response	parse_io_file_here(t_token **token, bool required)
{
	const t_response	operator = parse_token_type(token, is_redirection,
			required);
	t_response			filename;

	if (operator.status != SUCCESS || operator.data == NULL)
		return (operator);
	filename = parse_token_type(token, is_word_or_here_eof, REQUIRED);
	if (filename.status != SUCCESS)
	{
		free_tree_and_non_source_tokens(operator.data, LEXER);
		return (filename);
	}
	((t_ast_node *)operator.data)->left = filename.data;
	return (operator);
}

t_response	parse_io_redirect(t_token **token)
{
	const t_response	io_number = parse_token_type(token, is_io_number,
			OPTIONAL);
	const bool			require_io_file = (io_number.data != NULL);
	t_response			io_file_here;

	if (io_number.status != SUCCESS)
		return (io_number);
	io_file_here = parse_io_file_here(token, require_io_file);
	if (io_file_here.status == SUCCESS && io_file_here.data != NULL)
		((t_ast_node *)io_file_here.data)->left->left = io_number.data;
	else
		free_tree_and_non_source_tokens(io_number.data, LEXER);
	return (io_file_here);
}
