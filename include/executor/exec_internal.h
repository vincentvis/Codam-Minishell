/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_internal.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/09 19:04:22 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/05/27 09:38:52 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_INTERNAL_H
# define EXEC_INTERNAL_H

# include "parser/parser.h"
# include "utils/vector.h"
# include "utils/status.h"
# include <sys/types.h>

# define READ_FD 0
# define WRITE_FD 1

enum e_smingleton_actions {
	GET_ARRAY,
	CLEAN
};

t_response	get_command_path(const char *cmd);
t_response	resolve_redirections_tree(const t_ast_node *tree);
char		**collect_arguments(const t_ast_node *command);
t_vect		*p_id_singleton(enum e_smingleton_actions action);
void		kill_children(int sig);
t_vect		*save_pid(pid_t p_id);
pid_t		get_last_pid(void);
void		clean_pids(void);
int			is_builtin(const char *cmd) __attribute__((always_inline));
int			open_file(const char *string, const int mode);
int			determine_fd(const t_ast_node *redir_head);
t_response	execute_pipe_sequence(const t_ast_node *tree, int in_fd);
void		wait_for_exec_finish(void);
t_response	setup_and_execute_builtin(
				const t_ast_node *tree,
				bool on_pipe,
				int fildes[2],
				int in_fd
				);
void		set_up_and_execute_command(
				const t_ast_node *command,
				bool on_pipe,
				int fildes[2],
				int in_fd
				);

#endif
