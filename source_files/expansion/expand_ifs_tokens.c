/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_ifs_tokens.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/08 16:38:41 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/02 16:06:55 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "expansion/expand_functions.h"
#include "lexer/token.h"

static t_token	*allocate_token_with_length(t_chunk info)
{
	t_token	*token;

	token = malloc(sizeof(*token));
	if (token == NULL)
		return (NULL);
	token->content.expanded_word = ft_calloc(info.total_length + 1,
			sizeof(*token->content.expanded_word));
	if (token->content.expanded_word == NULL)
	{
		free(token);
		return (NULL);
	}
	token->type = WORD_TYPE;
	token->allocated = true;
	token->source = EXPANSION;
	token->next = NULL;
	token->length = info.total_length;
	return (token);
}

t_token	*token_create_from_many(t_chunk info)
{
	t_token	*token;
	char	*dst;
	size_t	chunk_length;

	token = allocate_token_with_length(info);
	if (!token)
		return (NULL);
	dst = token->content.expanded_word;
	while (info.total_length > 0)
	{
		chunk_length = ft_strlcpy(dst, info.start, info.total_length + 1);
		if (chunk_length >= info.total_length)
			break ;
		info.total_length -= chunk_length;
		dst += chunk_length;
		info.start = info.tokens->content.expanded_word;
		info.tokens = info.tokens->next;
	}
	token->source = EXPANSION;
	return (token);
}
