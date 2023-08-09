/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   generate_ast_dotfile.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/23 16:14:51 by vvissche      #+#    #+#                 */
/*   Updated: 2022/05/06 09:36:02 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "debug/minishell_debug.nonorm"
#include "utils/status.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static void	write_safe_label_name(const int fd, t_token *token)
{
	size_t	iterator;

	iterator = 0;
	while (token->content.raw_word[iterator] != '\0'
		&& iterator < token->length)
	{
		if (token->content.raw_word[iterator] == '"')
			write(fd, "\\", 1);
		write(fd, &(token->content.raw_word[iterator]), 1);
		iterator++;
	}
}

static void	write_token_relations(const int fd, t_ast_node *node)
{
	if (node)
	{
		if (node->left)
		{
			ft_dprintf(fd, "\t\"%p\" -> \"%p\" [label=\"L\"]\n",
				(void*)node, (void*)node->left);
			write_token_relations(fd, node->left);
		}
		if (node->right)
		{
			ft_dprintf(fd, "\t\"%p\" -> \"%p\" [label=\"R\"]\n",
				(void*)node, (void*)node->right);
			write_token_relations(fd, node->right);
		}
	}
}

static void	write_token_labels(const int fd, t_ast_node *node)
{
	if (node)
	{
		if (node->token && node->token->content.raw_word)
		{
			ft_dprintf(fd, "\t\t\"%p\" [label=\"", (void *)node);
			write_safe_label_name(fd, node->token);
			ft_dprintf(fd, "\"]\n");
		}
		if (node->left)
			write_token_labels(fd, node->left);
		if (node->right)
			write_token_labels(fd, node->right);
	}
}

void	create_dotfile_from_ast(t_ast_node *tree)
{
	int	fd;

	fd = open("ast.dot", O_RDWR | O_CREAT | O_TRUNC,
			S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd > 0)
	{
		ft_dprintf(fd, "digraph {\n\t{\n");
		write_token_labels(fd, tree);
		ft_dprintf(fd, "\t}\n");
		write_token_relations(fd, tree);
		ft_dprintf(fd, "}\n");
		close(fd);
	}
}
