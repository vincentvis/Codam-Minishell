/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/13 22:53:46 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/27 13:22:01 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "utils/status.h"

void		clean_heredoc(const t_ast_node *tree);
t_response	setup_heredocs_in_tree(const t_ast_node *tree);
int			get_heredoc_input_in_child(const char *eof, int fd_write,
				bool should_expand);

#endif
