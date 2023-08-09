/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ast.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/07/15 17:00:03 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/07 18:17:23 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "lexer/token.h"
#include "parser/parser.h"

void	*free_token(t_token *token)
{
	if (token->allocated)
	{
		free(token->content.expanded_word);
		token->content.expanded_word = NULL;
		token->allocated = false;
	}
	free(token);
	token = NULL;
	return (NULL);
}

void	*free_tree_and_non_source_tokens(
		t_ast_node *node,
		enum e_token_source source
	)
{
	if (node)
	{
		if (node->left)
			node->left = free_tree_and_non_source_tokens(node->left, source);
		if (node->right)
			node->right = free_tree_and_non_source_tokens(node->right, source);
		if (node->token && node->token->source != source)
			node->token = free_token(node->token);
		free(node);
		node = NULL;
	}
	return (NULL);
}

void	*free_tree(t_ast_node *node)
{
	if (node)
	{
		if (node->left)
			node->left = free_tree(node->left);
		if (node->right)
			node->right = free_tree(node->right);
		free(node);
		node = NULL;
	}
	return (NULL);
}
