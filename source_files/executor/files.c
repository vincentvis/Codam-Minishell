/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   files.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/27 09:36:22 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 20:00:29 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "parser/parser.h"
#include "parser/validation.h"

int	open_file(const char *string, const int mode)
{
	static const int	map[__INTERNAL_TYPE_END] = {
	[LESS_TYPE] = O_RDONLY,
	[GREAT_TYPE] = O_RDWR | O_CREAT | O_TRUNC,
	[DGREAT_TYPE] = O_RDWR | O_CREAT | O_APPEND,
	[DLESS_TYPE] = O_WRONLY | O_CREAT | O_TRUNC,
	[HERE_EOF_TYPE] = O_RDONLY
	};

	return (open(string, map[mode], 0644));
}

int	determine_fd(const t_ast_node *redir_head)
{
	if (!is_redirection(redir_head->token->type))
		return (-1);
	if (redir_head->left->left)
		return (redir_head->left->left->token->content.digit);
	else if (redir_head->token->type == LESS_TYPE
		|| redir_head->token->type == DLESS_TYPE)
		return (STDIN_FILENO);
	else
		return (STDOUT_FILENO);
}
