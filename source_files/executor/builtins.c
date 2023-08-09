/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/20 12:59:25 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/08 09:54:07 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "utils/status.h"
#include "parser/parser.h"
#include "builtins/builtins.h"
#include "executor/exec_internal.h"
#include "utils/exitcode.h"

int	is_builtin(const char *cmd)
{
	return (ft_memcmp("unset", cmd, 6) == 0
		|| ft_memcmp("env", cmd, 4) == 0
		|| ft_memcmp("pwd", cmd, 4) == 0
		|| ft_memcmp("cd", cmd, 3) == 0
		|| ft_memcmp("echo", cmd, 5) == 0
		|| ft_memcmp("export", cmd, 7) == 0
		|| ft_memcmp("exit", cmd, 5) == 0);
}

//MUST BE BUILT_IN!!! 
int	(*get_builtin_function(char *name))(char *const *args)
{
	if (*name == 'c')
		return (&builtin_cd);
	if (*name == 'e' && name[1] == 'c')
		return (&builtin_echo);
	if (*name == 'e' && name[1] == 'n')
		return (&builtin_env);
	if (*name == 'e' && name[2] == 'i')
		return (&builtin_exit);
	if (*name == 'e' && name[2] == 'p')
		return (&builtin_export);
	if (*name == 'p')
		return (&builtin_pwd);
	return (&builtin_unset);
}

static t_response	builtin_redirections(
	const t_ast_node *tree,
	bool on_pipe,
	int fildes[2],
	int in_fd)
{
	if (on_pipe)
	{
		if (dup2(fildes[WRITE_FD], STDOUT_FILENO) == -1)
			return (make_response(E_FD_DUPLICATION_FAILURE, "dup2"));
		if (close(fildes[WRITE_FD]) == -1)
			return (make_response(E_CLOSE_FAILURE, "close"));
	}
	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			return (make_response(E_FD_DUPLICATION_FAILURE, "dup2"));
		if (close(in_fd) == -1)
			return (make_response(E_CLOSE_FAILURE, "close"));
	}
	return (resolve_redirections_tree(tree));
}

static t_response	after_builtin_execution(
	int builtin_return_value,
	char **arguments,
	int restore[2],
	bool on_pipe)
{
	pid_t	p_id;

	free(arguments);
	arguments = NULL;
	p_id = fork();
	if (p_id == -1)
		return (make_response(E_FORK_FAILURE, "fork"));
	if (p_id == 0)
		exit(builtin_return_value);
	else if (save_pid(p_id) == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	if (dup2(restore[READ_FD], STDIN_FILENO) == -1
		|| dup2(restore[WRITE_FD], STDOUT_FILENO) == -1)
		return (make_response(E_FD_DUPLICATION_FAILURE, "dup2"));
	if (close(restore[READ_FD]) == -1 || close(restore[WRITE_FD]) == -1)
		return (make_response(E_CLOSE_FAILURE, "close"));
	if (!on_pipe)
		wait_for_exec_finish();
	return (make_response(SUCCESS, NULL));
}

t_response	setup_and_execute_builtin(const t_ast_node *tree, bool on_pipe,
	int fildes[2], int in_fd)
{
	const int	std_in_restore = dup(STDIN_FILENO);
	const int	std_out_restore = dup(STDOUT_FILENO);
	char		**arguments;
	t_response	response;

	if (std_in_restore == -1 || std_out_restore == -1)
		return (make_response(E_FD_DUPLICATION_FAILURE, "dup"));
	arguments = collect_arguments(tree);
	if (arguments == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	response = builtin_redirections(tree, on_pipe, fildes, in_fd);
	if (response.status != SUCCESS)
	{
		dup2(std_in_restore, STDIN_FILENO);
		dup2(std_out_restore, STDOUT_FILENO);
		store_exit_code(1);
		return (response);
	}
	return (after_builtin_execution(
			get_builtin_function(tree->token->content.expanded_word)(arguments),
		arguments,
		(int [2]){std_in_restore, std_out_restore},
		on_pipe
		));
}
