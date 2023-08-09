/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/26 17:45:24 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/09 09:43:56 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "lexer/token.h"
#include "libft.h"

t_token_type	token_determine_type(const t_state state)
{
	static const t_token_type	types[INTERNAL_END_STATE] = {
	[WORD_STATE] = WORD_TYPE,
	[DIGIT_STATE] = IO_NUMBER_TYPE,
	[PIPE_STATE] = PIPE_TYPE,
	[LESS_STATE] = LESS_TYPE,
	[GREAT_STATE] = GREAT_TYPE,
	[DGREAT_STATE] = DGREAT_TYPE,
	[DLESS_STATE] = DLESS_TYPE,
	[DQUOTE_END] = DQUOTE_TYPE,
	[QUOTE_END] = QUOTE_TYPE,
	[VAR_START_STATE] = DOLLARSIGN_TYPE,
	[VAR_ARG_STATE] = VAR_TYPE,
	[IN_VAR_STATE] = VAR_TYPE,
	[EXITCODE_STATE] = EXITCODE_TYPE,
	};

	return (types[state]);
}

t_token	*token_create_ionumber_type(int number)
{
	t_token	*token;

	token = malloc(sizeof(*token));
	if (!token)
		return (NULL);
	token->type = IO_NUMBER_TYPE;
	token->content.digit = number;
	token->length = 0;
	token->allocated = false;
	token->next = NULL;
	return (token);
}

t_token	*token_create_raw(
		const t_token_type type,
		const char *content,
		const size_t length,
		const bool allocated
	)
{
	t_token	*token;

	token = malloc(sizeof(*token));
	if (!token)
		return (NULL);
	token->type = type;
	token->content.raw_word = content;
	token->length = length;
	token->allocated = allocated;
	token->source = UNKNOWN;
	token->next = NULL;
	return (token);
}

void	token_free_list(t_token *head)
{
	if (!head)
		return ;
	if (head->next)
	{
		token_free_list(head->next);
		head->next = NULL;
	}
	if (head->allocated && head->content.expanded_word)
	{
		free(head->content.expanded_word);
		head->content.expanded_word = NULL;
		head->allocated = false;
	}
	free(head);
}
