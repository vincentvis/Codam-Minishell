/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/13 17:29:15 by vvissche      #+#    #+#                 */
/*   Updated: 2022/06/13 09:53:24 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_printf.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "utils/exitcode.h"
#include "utils/signals.h"
#include "executor/exec_internal.h"
#include "executor/heredoc.h"

static int	fork_for_heredoc(
		t_ast_node *node,
		const char *eof,
		int fd_write,
		char *tmp_file
	)
{
	const bool	should_expand = node->token->type == HERE_EOF_EXP_TYPE;
	pid_t		p_id;
	int			exit_status;

	p_id = fork();
	if (p_id == -1)
		return (E_FORK_FAILURE);
	if (p_id == 0)
		exit(get_heredoc_input_in_child(eof, fd_write, should_expand));
	else
	{
		waitpid(p_id, &exit_status, 0);
		if (WIFSIGNALED(exit_status))
			return (E_HERE_FAILURE);
		node->right = ast_make_node(
				token_create_ionumber_type(open_file(tmp_file, HERE_EOF_TYPE)));
	}
	return (SUCCESS);
}

static int	setup_heredoc_for_ast_node(const t_ast_node *node)
{
	const char	*eof = node->left->token->content.expanded_word;
	char		tmp_file[42];
	int			fd_write;
	int			status;

	if (ft_sprintf(tmp_file, "/tmp/sauropod_%p_%.10s", (void *)eof, eof) == -1)
		return (MALLOC_FAILURE);
	fd_write = open_file(tmp_file, DLESS_TYPE);
	if (fd_write == -1)
		return (E_HERE_FILE_FAILURE);
	set_signals_for_heredoc_parent();
	status = fork_for_heredoc(node->left, eof, fd_write, tmp_file);
	close(fd_write);
	unlink(tmp_file);
	if (status != SUCCESS)
		return (status);
	if (node->left->right == NULL
		|| node->left->right->token == NULL)
		return (MALLOC_FAILURE);
	if (node->left->right->token->content.digit == -1)
		return (E_FILE_FAILURE);
	node->left->right->token->source = HEREDOC;
	return (SUCCESS);
}

int	is_dless_with_open_fd(const t_ast_node *node)
{
	return (node->token
		&& node->token->type == DLESS_TYPE
		&& node->left
		&& node->left->right
		&& node->left->right->token);
}

void	clean_heredoc(const t_ast_node *tree)
{
	if (tree == NULL)
		return ;
	if (tree->right)
		clean_heredoc(tree->right);
	if (is_dless_with_open_fd(tree))
	{
		close(tree->left->right->token->content.digit);
		tree->left->right->token->content.digit = -1;
		free(tree->left->right);
		tree->left->right = NULL;
		return ;
	}
	if (tree->left)
		clean_heredoc(tree->left);
}

t_response	setup_heredocs_in_tree(const t_ast_node *tree)
{
	int			status;
	t_response	response;

	if (tree == NULL)
		return (make_response(SUCCESS, NULL));
	if (tree->token && tree->token->type == DLESS_TYPE)
	{
		status = setup_heredoc_for_ast_node(tree);
		if (status != SUCCESS && store_exit_code(1))
			return (make_response(status, NULL));
	}
	if (tree->left)
	{
		response = setup_heredocs_in_tree(tree->left);
		if (response.status != SUCCESS && store_exit_code(1))
			return (response);
	}
	if (tree->right)
	{
		response = setup_heredocs_in_tree(tree->right);
		if (response.status != SUCCESS && store_exit_code(1))
			return (response);
	}
	return (make_response(SUCCESS, NULL));
}
