/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   var.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/13 09:33:26 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/13 09:33:32 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexer/token.h"
#include "env/she_env.h"
#include "utils/status.h"

t_response	expand_var_type(t_token *token)
{
	t_response	get_env;
	char		*key;

	key = ft_substr(token->content.raw_word, 1, token->length - 1);
	if (key == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	get_env = sheenv_get_value_by_key(key);
	free(key);
	key = NULL;
	if (token->allocated)
		free(token->content.expanded_word);
	if (get_env.status != SUCCESS)
		token->content.expanded_word = ft_strdup("");
	else
		token->content.expanded_word = ft_strdup((char *)get_env.data);
	if (token->content.expanded_word == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	token->length = ft_strlen(token->content.expanded_word);
	token->allocated = true;
	token->type = WORD_TYPE;
	return (make_response(SUCCESS, token));
}
