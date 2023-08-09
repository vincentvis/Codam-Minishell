/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_command.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/26 16:38:51 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 23:11:43 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "env/she_env.h"
#include "parser/parser.h"
#include "executor/exec_internal.h"

int	is_directory(char *cmd_path)
{
	struct stat	path_stat;

	stat(cmd_path, &path_stat);
	return (S_ISDIR(path_stat.st_mode));
}

//what happens if you execute an empty tree...
void	execute_command(const t_ast_node *tree, const char **env)
{
	t_response	cmd_path_response;
	const char	*cmd_name;
	char		*cmd_path;
	char		**argv;

	if (tree->token->type != WORD_TYPE)
		exit(0);
	argv = collect_arguments(tree);
	if (!argv)
		perror_and_exit("");
	cmd_name = argv[0];
	cmd_path_response = get_command_path(cmd_name);
	if (cmd_path_response.status != SUCCESS)
		perror_and_exit("");
	cmd_path = cmd_path_response.data;
	if (cmd_path == NULL)
		error_and_exit(cmd_name, "command not found", EXIT_CODE_NOT_FOUND);
	if (is_directory(cmd_path))
		error_and_exit(cmd_name, "is a directory", EXIT_CODE_CANNOT_EXECUTE);
	execve(cmd_path, argv, (char *const *)env);
	free(cmd_path);
	free(argv);
	perror_and_exit(cmd_name);
}

pid_t	execute_command_parent(const t_ast_node *tree)
{
	t_response			response;
	const t_response	env_response = sheenv_to_array(false);
	const char			**env = env_response.data;

	if (env_response.status != SUCCESS)
		perror_and_exit("");
	response = resolve_redirections_tree(tree);
	if (response.status != SUCCESS)
	{
		manage_error(response.status, response.data);
		exit(1);
	}
	execute_command(tree, env);
	return (0);
}

void	set_up_and_execute_command(const t_ast_node *command, bool on_pipe,
		int fildes[2], int in_fd)
{
	if (on_pipe)
	{
		if (close(fildes[READ_FD]) == -1)
			perror_and_exit("");
		if (dup2(fildes[WRITE_FD], STDOUT_FILENO) == -1)
			perror_and_exit("");
		if (close(fildes[WRITE_FD]) == -1)
			perror_and_exit("");
	}
	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			perror_and_exit("");
		if (close(in_fd) == -1)
			perror_and_exit("");
	}
	execute_command_parent(command);
}
