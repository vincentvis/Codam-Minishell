/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   update_tree.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/12 21:54:27 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 16:42:48 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "libft.h"
#include "parser/parser.h"
#include "lexer/token.h"
#include "utils/status.h"
#include "expansion/expansion.h"
#include "expansion/expand_functions.h"

static t_response	update_three_with_expanded_tokens(t_token *new_tokens,
	t_ast_node *node)
{
	t_ast_node *const	right = node->right;
	t_ast_node *const	head = node;
	t_token *const		first_token = new_tokens;
	t_ast_node			*new_node;
	t_ast_node			*new_nodes_head;

	if (new_tokens == NULL)
		return (make_response(SUCCESS, NULL));
	new_tokens = new_tokens->next;
	new_nodes_head = NULL;
	while (new_tokens)
	{
		new_node = ast_make_node_and_consume(&new_tokens);
		if (new_node == NULL && free_tree(new_nodes_head) == NULL)
			head->right = right;
		if (new_node == NULL)
			return (make_response(MALLOC_FAILURE, NULL));
		if (new_nodes_head == NULL)
			new_nodes_head = new_node;
		node->right = new_node;
		node = node->right;
	}
	head->token = first_token;
	node->right = right;
	return (make_response(SUCCESS, head));
}

static t_response	expand_token_content_source(t_token *token,
	enum e_token_source source)
{
	t_response	expanded_tokens;
	t_token		*set_source;

	expanded_tokens = expand_token_content(token);
	if (expanded_tokens.status != SUCCESS)
		return (expanded_tokens);
	set_source = expanded_tokens.data;
	while (set_source)
	{
		set_source->source = source;
		set_source = set_source->next;
	}
	return (expanded_tokens);
}

static t_response	expand_tree_token(t_ast_node **head)
{
	t_response	expanded_tokens;
	t_response	updated_tree;

	expanded_tokens = expand_token_content_source((*head)->token, UPDATE_TREE);
	if (expanded_tokens.status != SUCCESS)
		return (expanded_tokens);
	if (expanded_tokens.data == NULL)
		return (make_response(SUCCESS, (*head)->right));
	updated_tree = update_three_with_expanded_tokens(expanded_tokens.data,
			*head);
	if (updated_tree.status != SUCCESS)
	{
		token_free_list(expanded_tokens.data);
		return (updated_tree);
	}
	return (updated_tree);
}

static bool	should_expand_token(t_token_type type)
{
	return (type == WORD_TYPE
		|| type == HERE_EOF_TYPE
		|| type == HERE_EOF_EXP_TYPE);
}

t_response	expand_tree(t_ast_node **head)
{
	t_response	updated_tree;

	if (!*head)
		return (make_response(SUCCESS, NULL));
	if ((*head)->right)
	{
		updated_tree = expand_tree(&((*head)->right));
		if (updated_tree.status != SUCCESS)
			return (updated_tree);
		(*head)->right = updated_tree.data;
	}
	if ((*head)->left)
	{
		updated_tree = expand_tree(&((*head)->left));
		if (updated_tree.status != SUCCESS)
			return (updated_tree);
		(*head)->left = updated_tree.data;
	}
	if ((*head)->token && should_expand_token((*head)->token->type))
		return (expand_tree_token(head));
	return (make_response(SUCCESS, *head));
}
