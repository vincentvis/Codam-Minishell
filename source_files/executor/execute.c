/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/08 20:07:41 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/07 18:08:12 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdbool.h>
#include "parser/parser.h"
#include "executor/exec_internal.h"
#include "utils/exitcode.h"
#include "executor/heredoc.h"
#include "utils/interactive.h"

void	wait_for_exec_finish(void)
{
	pid_t		p_id;
	int			exit_status;

	p_id = get_last_pid();
	if (p_id)
	{
		waitpid(p_id, &exit_status, 0);
		if (WIFSIGNALED(exit_status))
			store_exit_code(128 + WTERMSIG(exit_status));
		else if (WIFEXITED(exit_status))
			store_exit_code(WEXITSTATUS(exit_status));
		while (wait(NULL) > 0)
			;
	}
}

static void	cleanup_exec(const t_ast_node *tree)
{
	if (tree)
		clean_heredoc(tree);
	clean_pids();
}

t_response	execute(const t_ast_node *tree)
{
	t_response	response;

	if (!tree)
		return (make_response(SUCCESS, NULL));
	response = setup_heredocs_in_tree(tree);
	if (response.status != SUCCESS)
		return (response);
	if (tree->token->type != PIPE_TYPE
		&& is_builtin(tree->token->content.expanded_word))
	{
		cleanup_exec(tree);
		return (setup_and_execute_builtin(tree, false, NULL, STDIN_FILENO));
	}
	set_not_interactive();
	response = execute_pipe_sequence(tree, STDIN_FILENO);
	if (response.status != SUCCESS)
	{
		cleanup_exec(tree);
		store_exit_code(1);
		return (response);
	}
	wait_for_exec_finish();
	set_interactive();
	cleanup_exec(tree);
	return (make_response(SUCCESS, NULL));
}
