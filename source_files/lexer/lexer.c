/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/28 22:08:18 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/07 18:16:30 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/token.h"
#include "lexer/validation.h"
#include "lexer/state.h"
#include "libft.h"
#include "utils/status.h"

static const t_transition	*get_transition_table(void)
{
	static const t_transition	transitions[] = {
	{START_STATE, LESS_STATE, &isleftchevron},
	{START_STATE, GREAT_STATE, &isrightchevron},
	{START_STATE, PIPE_STATE, &ispipe},
	{START_STATE, IN_QUOTE_STATE, &isquote},
	{START_STATE, IN_DQUOTE_STATE, &isdquote},
	{START_STATE, DIGIT_STATE, &ft_isdigit},
	{START_STATE, WORD_STATE, &isword},
	{START_STATE, START_STATE, &ft_isspace},
	{IN_QUOTE_STATE, IN_QUOTE_STATE, &isinsidequote},
	{IN_QUOTE_STATE, WORD_STATE, &isquote},
	{IN_DQUOTE_STATE, IN_DQUOTE_STATE, &isinsidedquote},
	{IN_DQUOTE_STATE, WORD_STATE, &isdquote},
	{WORD_STATE, IN_QUOTE_STATE, &isquote},
	{WORD_STATE, IN_DQUOTE_STATE, &isdquote},
	{WORD_STATE, WORD_STATE, &isword},
	{DIGIT_STATE, DIGIT_STATE, &ft_isdigit},
	{DIGIT_STATE, IN_QUOTE_STATE, &isquote},
	{DIGIT_STATE, IN_DQUOTE_STATE, &isdquote},
	{DIGIT_STATE, WORD_STATE, &isword},
	{LESS_STATE, DLESS_STATE, &isleftchevron},
	{GREAT_STATE, DGREAT_STATE, &isrightchevron},
	{INTERNAL_END_STATE, INTERNAL_END_STATE, NULL}};

	return (transitions);
}

static t_state	get_next_state(
		const char *line,
		const t_state state,
		const t_transition *transitions
	)
{
	while (transitions->current != INTERNAL_END_STATE)
	{
		if (transitions->current == state)
		{
			if (transitions->func(*line))
				break ;
		}
		transitions++;
	}
	return (transitions->next);
}

static t_response	lexer_manage_error(t_token *head, enum e_status_code error)
{
	token_free_list(head);
	return (make_response(error, NULL));
}

t_response	lex_and_make_tokens(
		const char *line,
		const t_state state,
		const t_transition *const transitions
	)
{
	t_token				*token;
	t_response			response;
	const t_state		new_state = get_next_state(line, state, transitions);
	static const char	*begin;

	if (state == START_STATE)
		begin = line;
	if (new_state != INTERNAL_END_STATE)
		return (lex_and_make_tokens(line + 1, new_state, transitions));
	if (state > internal_VALID_TOKEN_END_STATE)
	{
		token = token_create_raw(token_determine_type(state),
				begin, (size_t)(line - begin), false);
		if (token == NULL)
			return (lexer_manage_error(NULL, MALLOC_FAILURE));
		response = lex_and_make_tokens(line, START_STATE, transitions);
		if (response.status != 0)
			return (lexer_manage_error(token, response.status));
		token->next = response.data;
		return (make_response(SUCCESS, token));
	}
	else if (state == START_STATE && *line == '\0')
		return (make_response(SUCCESS, NULL));
	else
		return (lexer_manage_error(NULL, L_BAD_INPUT));
}

t_response	lex_input(const char *line)
{
	t_response	tokens;

	tokens = lex_and_make_tokens(line, START_STATE, get_transition_table());
	if (tokens.status == SUCCESS)
		token_review_lexed_types(tokens.data);
	return (tokens);
}
