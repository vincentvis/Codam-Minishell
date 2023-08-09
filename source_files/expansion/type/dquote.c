/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   dquote.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/13 09:25:29 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 18:12:19 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "utils/status.h"
#include "expansion/expand_validation.h"
#include "expansion/expand_functions.h"
#include "lexer/lexer_private.h"
#include "lexer/token.h"
#include "lexer/validation.h"

static const t_transition	*get_transition_table(void)
{
	static const t_transition	transitions[] = {
	{START_STATE, VAR_START_STATE, &isdollarsign},
	{START_STATE, WORD_STATE, &isinsidedquote},
	{VAR_START_STATE, IN_VAR_STATE, &isvarstart},
	{VAR_START_STATE, EXITCODE_STATE, &isquestionmark},
	{VAR_START_STATE, VAR_ARG_STATE, &isdigit_arg},
	{VAR_START_STATE, IN_VAR_STATE, &isvarstart},
	{IN_VAR_STATE, IN_VAR_STATE, &isinsidevar},
	{WORD_STATE, WORD_STATE, &iswordindquote},
	{INTERNAL_END_STATE, INTERNAL_END_STATE, NULL}
	};

	return (transitions);
}

t_response	expand_var_tokens_in_list(t_token *token)
{
	t_token		*start;
	t_response	expand_response;

	start = token;
	while (token)
	{
		if (token->type == VAR_TYPE)
		{
			expand_response = expand_var_type(token);
			if (expand_response.status != SUCCESS)
				return (expand_response);
			token = expand_response.data;
		}
		if (token->type == EXITCODE_TYPE)
		{
			expand_response = expand_exitcode_type(token);
			if (expand_response.status != SUCCESS)
				return (expand_response);
			token = expand_response.data;
		}
		token = token->next;
	}
	return (make_response(SUCCESS, start));
}

t_response	join_token_list(t_token *start)
{
	t_token		*token;
	size_t		total_length;
	size_t		offset;
	char		*joined_string;

	token = start;
	total_length = 0;
	while (token)
	{
		total_length += token->length;
		token = token->next;
	}
	joined_string = ft_calloc(total_length + 1, sizeof(*joined_string));
	if (joined_string == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	token = start;
	offset = 0;
	while (token)
	{
		ft_memcpy(&joined_string[offset], token->content.expanded_word,
			token->length);
		offset += token->length;
		token = token->next;
	}
	return (make_response(SUCCESS, joined_string));
}

static t_response	reconstruct_dquote_contents(char *duplicate)
{
	t_response	dquote_tokens;
	t_response	process_inner_tokens;
	t_response	tokens_joined_to_string;

	dquote_tokens = lex_and_make_tokens(duplicate, START_STATE,
			get_transition_table());
	if (dquote_tokens.status != SUCCESS)
	{
		free(duplicate);
		return (dquote_tokens);
	}
	process_inner_tokens = expand_var_tokens_in_list(dquote_tokens.data);
	if (process_inner_tokens.status != SUCCESS)
	{
		token_free_list(dquote_tokens.data);
		return (process_inner_tokens);
	}
	tokens_joined_to_string = join_token_list(dquote_tokens.data);
	token_free_list(dquote_tokens.data);
	return (tokens_joined_to_string);
}

t_response	expand_dquote_type(t_token *token)
{
	char		*duplicate;
	t_response	tokens_joined_to_string;

	duplicate = ft_substr(token->content.expanded_word, 1, token->length - 2);
	if (duplicate == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	tokens_joined_to_string = reconstruct_dquote_contents(duplicate);
	free(duplicate);
	if (tokens_joined_to_string.status != SUCCESS)
		return (tokens_joined_to_string);
	token->content.expanded_word = tokens_joined_to_string.data;
	token->allocated = true;
	token->length = ft_strlen(token->content.expanded_word);
	return (make_response(SUCCESS, token));
}
