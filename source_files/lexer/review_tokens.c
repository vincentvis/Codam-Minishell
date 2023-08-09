/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   review_tokens.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/17 19:57:07 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 18:18:26 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "lexer/token.h"
#include "libft.h"

static int	has_quote_char(const char *word, size_t	max)
{
	size_t	i;

	i = 0;
	while (word[i] && i < max)
	{
		if (word[i] == '"' || word[i] == '\'')
			return (1);
		i++;
	}
	return (0);
}

void	token_review_lexed_types(t_token *tokens)
{
	while (tokens)
	{
		tokens->source = LEXER;
		if (tokens->type == IO_NUMBER_TYPE)
		{
			if (tokens->next == NULL || tokens->next->type < _REDIRECTION
				|| ft_isspace(tokens->content.raw_word[tokens->length]))
				tokens->type = WORD_TYPE;
		}
		if (tokens->type == DLESS_TYPE && tokens->next
			&& tokens->next->type == WORD_TYPE)
		{
			if (has_quote_char(tokens->next->content.raw_word,
					tokens->next->length))
				tokens->next->type = HERE_EOF_TYPE;
			else
				tokens->next->type = HERE_EOF_EXP_TYPE;
		}
		tokens = tokens->next;
	}
}
