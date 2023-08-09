/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print_information.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/07 12:40:21 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/30 17:47:19 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "debug/minishell_debug.nonorm"

void	print_ast(const t_ast_node *tree, int indent, char *extra)
{
	int	i = 0;
	if (!tree)
		return ;
	if (tree->right)
		print_ast(tree->right, indent + 1, "┌");
	while (i < indent - 1)
	{
		ft_printf("\t");
		i++;
	}
	if (indent != 0)
		ft_printf("%s--- ", extra);
	if (tree->token)
	{
		if (tree->token->type == IO_NUMBER_TYPE)
		{
			ft_printf("%i\n", tree->token->content.digit);
		}
		else
		{
			ft_printf("%.*s\n", (int)tree->token->length,
				tree->token->content.raw_word);
		}
	}
	if (tree->left)
		print_ast(tree->left, indent + 1, "└");
}

void	print_tokens(const t_token *token)
{
	int					i;

	i = 0;
	while (token)
	{
		ft_printf("Token %i (%p), type:[%s]. Content: (%.*s) (%p) next: %p\n",
			i, (void*)token, debug_token_type_from_enum(token->type), (int)token->length,
			token->content.raw_word, (void*)token->content.raw_word,
			(void*)token->next);
		i++;
		token = token->next;
	}
}
