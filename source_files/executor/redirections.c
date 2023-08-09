/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirections.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/11 15:36:29 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/13 09:38:30 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include "parser/parser.h"
#include "parser/validation.h"
#include "executor/exec_internal.h"

//assumes it's a redirection, will break otherwise
static t_response	redirect_file(const t_ast_node *tree)
{
	const char	*filename = tree->left->token->content.expanded_word;
	const int	file_mode_indicator = tree->token->type;
	const int	file = open_file(filename, file_mode_indicator);

	if (file == -1)
		return (make_response(E_FILE_FAILURE,
				tree->left->token->content.expanded_word));
	if (dup2(file, determine_fd(tree)) == -1)
		return (make_response(E_FD_DUPLICATION_FAILURE, "dup2"));
	close(file);
	return (make_response(SUCCESS, NULL));
}

static t_response	redirect_heredoc(const t_ast_node *tree)
{
	int	here_fd;

	if (!(tree->left && tree->left->right && tree->left->right->token))
		return (make_response(SUCCESS, NULL));
	here_fd = tree->left->right->token->content.digit;
	if (dup2(here_fd, determine_fd(tree)) == -1)
		return (make_response(E_FD_DUPLICATION_FAILURE, "dup2"));
	return (make_response(SUCCESS, NULL));
}

// jeeeeeeeeeeez this is not making it better.. thanks norm!
static int	redir_action(
		t_response (*next_action)(const t_ast_node *tree),
		const t_ast_node *tree,
		t_response *response)
{
	*response = next_action(tree);
	return ((*response).status);
}

// this function assumes you feed it a command head
// maybe run recursion only for words
t_response	resolve_redirections_tree(const t_ast_node *tree)
{
	t_response	response;

	while (tree)
	{
		if (!is_redirection(tree->token->type) && tree->left
			&& redir_action(resolve_redirections_tree, tree->left, &response))
			return (response);
		if (tree->token->type == DLESS_TYPE
			&& redir_action(redirect_heredoc, tree, &response))
			return (response);
		if (is_redirection(tree->token->type)
			&& tree->token->type != DLESS_TYPE)
		{
			if (tree->left == NULL)
				return (make_response(E_AMBIGUOUS_REDIRECT,
						(void *)tree->token->content.raw_word));
			response = redirect_file(tree);
			if (response.status != SUCCESS)
				return (response);
		}
		tree = tree->right;
	}
	return (make_response(SUCCESS, NULL));
}
