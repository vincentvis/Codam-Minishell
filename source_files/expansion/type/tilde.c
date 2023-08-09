/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tilde.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/13 09:33:28 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/02 14:41:54 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexer/token.h"
#include "env/she_env.h"
#include "utils/status.h"
#include "lexer/validation.h"

void	is_first_token_tilde(t_token *token)
{
	if (token->type == WORD_TYPE && token->length == 1
		&& istilde(token->content.raw_word[0])
		&& (token->content.raw_word[1] == ' '
			|| token->content.raw_word[1] == '/'
			|| token->content.raw_word[1] == '\0'))
		token->type = TILDE_TYPE;
}

t_response	expand_tilde_type(t_token *token)
{
	const t_response	get_home = sheenv_get_value_by_key("HOME");
	char				*duplicate;

	if (get_home.status == SUCCESS)
		duplicate = ft_strdup(get_home.data);
	else
		duplicate = ft_substr(token->content.raw_word, 0, token->length);
	if (duplicate == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	token->content.expanded_word = duplicate;
	token->length = ft_strlen(duplicate);
	token->allocated = true;
	token->type = WORD_TYPE;
	return (make_response(SUCCESS, token));
}
