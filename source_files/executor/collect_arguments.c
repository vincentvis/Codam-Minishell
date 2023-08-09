/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   collect_arguments.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/08 17:21:05 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/05/15 16:22:01 by jowytrzy      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser/parser.h"

static size_t	count_words(const t_ast_node *tree)
{
	size_t		len;

	len = 0;
	while (tree)
	{
		if (tree->token->type == WORD_TYPE)
			len++;
		tree = tree->right;
	}
	return (len);
}

char	**collect_arguments(const t_ast_node *command)
{
	const size_t	len = count_words(command);
	char			**argv;
	size_t			i;

	argv = ft_calloc((len + 1), sizeof(*argv));
	if (!argv)
		return (NULL);
	i = 0;
	while (command)
	{
		if (command->token->type == WORD_TYPE)
		{
			argv[i] = command->token->content.expanded_word;
			i++;
		}
		command = command->right;
	}
	return (argv);
}
