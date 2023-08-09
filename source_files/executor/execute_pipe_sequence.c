/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_pipe_sequence.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/26 16:26:23 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 23:14:01 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "sys/types.h"
#include "parser/parser.h"
#include "utils/status.h"
#include "executor/exec_internal.h"
#include "utils/signals.h"

static t_response	execute_pipe_sequence_parent(const t_ast_node *tree,
	bool on_pipe,
	int fildes[2],
	pid_t p_id)
{
	if (save_pid(p_id) == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	if (on_pipe && close(fildes[WRITE_FD]) == -1)
		return (make_response(E_CLOSE_FAILURE, "close"));
	if (on_pipe)
		return (execute_pipe_sequence(tree->right, fildes[READ_FD]));
	return (make_response(SUCCESS, NULL));
}

static void	execute_pipe_sequence_child(const t_ast_node *command,
	bool on_pipe,
	int fildes[2],
	int in_fd)
{
	t_response	response;

	set_signals_for_child();
	if (is_builtin(command->token->content.expanded_word))
	{
		response = setup_and_execute_builtin(command, on_pipe, fildes, in_fd);
		if (response.status != SUCCESS)
		{
			manage_error(response.status, response.data);
			exit(1);
		}
		exit(0);
	}
	else
		set_up_and_execute_command(command, on_pipe, fildes, in_fd);
}

//for now that's what I need but it looks like a bad function which doesnt
//account for other situations
static const t_ast_node	*next_command(const t_ast_node *tree)
{
	if (tree->token->type == PIPE_TYPE)
		return (tree->left);
	return (tree);
}

t_response	execute_pipe_sequence(const t_ast_node *tree, int in_fd)
{
	int					fildes[2];
	pid_t				p_id;
	const t_ast_node	*command = next_command(tree);
	const bool			on_pipe = tree->token->type == PIPE_TYPE;

	if (!tree)
		return (make_response(SUCCESS, (void *)tree));
	if (on_pipe && pipe(fildes) == -1)
		return (make_response(E_PIPE_FAILURE, "pipe"));
	p_id = fork();
	if (p_id == -1)
		return (make_response(E_FORK_FAILURE, "fork"));
	if (p_id == 0)
		execute_pipe_sequence_child(command, on_pipe, fildes, in_fd);
	else
	{
		set_signals_for_parent();
		if (in_fd != STDIN_FILENO && close(in_fd) == -1)
			return (make_response(E_CLOSE_FAILURE, "close"));
		return (execute_pipe_sequence_parent(tree, on_pipe, fildes, p_id));
	}
	return (make_response(SUCCESS, NULL));
}
