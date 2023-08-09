/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   node.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/07/15 16:10:18 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/07 18:17:59 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/token.h"
#include "parser/parser.h"
#include "libft.h"

t_ast_node	*ast_make_node(t_token *const content)
{
	t_ast_node *const	new_node = ft_calloc(1, sizeof(t_ast_node));

	if (new_node)
		new_node->token = content;
	return (new_node);
}

t_ast_node	*ast_make_node_and_consume(t_token **tokens)
{
	t_ast_node *const	new_node = ast_make_node(*tokens);

	if (new_node && tokens && *tokens)
		*tokens = (*tokens)->next;
	return (new_node);
}
