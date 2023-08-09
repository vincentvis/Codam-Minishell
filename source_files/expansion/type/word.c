/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   word.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/13 09:29:35 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/09 10:10:27 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexer/token.h"
#include "utils/status.h"
#include "utils/minishell_colors.h"

t_response	expand_word_type(t_token *token)
{
	char	*duplicate;

	duplicate = ft_substr(token->content.raw_word, 0, token->length);
	if (duplicate == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	token->content.expanded_word = duplicate;
	token->allocated = true;
	return (make_response(SUCCESS, token));
}

t_response	expand_dollarsign_type(t_token *token)
{
	if (token->next)
	{
		token->content.expanded_word = ft_strdup(BRED">> STOP TESTING "
				"UNDEFINED BEHAVIOUR << "COLOR_OFF);
		if (token->content.expanded_word == NULL)
			return (make_response(MALLOC_FAILURE, NULL));
		token->length = 0;
		return (make_response(SUCCESS, token));
	}
	return (expand_word_type(token));
}
