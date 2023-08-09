/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   simple_quotes.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/13 09:30:55 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/13 09:32:27 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexer/token.h"
#include "utils/status.h"

t_response	expand_simple_quote_type(t_token *token)
{
	char	*duplicate;

	duplicate = ft_substr(token->content.raw_word, 1, token->length - 2);
	if (duplicate == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	token->content.expanded_word = duplicate;
	token->length = token->length - 2;
	token->allocated = true;
	return (make_response(SUCCESS, token));
}
