/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_ifs.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/08 16:38:41 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 18:14:11 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexer/token.h"
#include "utils/status.h"
#include "expansion/expand_functions.h"
#include "expansion/expand_validation.h"

static t_chunk	new_chunk(
		const char *start,
		const t_token *const tokens,
		const size_t len
	)
{
	t_chunk	chunk;

	chunk.start = start;
	chunk.tokens = tokens;
	chunk.total_length = len;
	return (chunk);
}

static t_pos	find_first_char(t_pos position, const char *ifs)
{
	t_token	*current_token;
	char	*line;

	current_token = position.current_token;
	line = position.line;
	while (current_token && (current_token->next || (line && *line)))
	{
		if (is_quote(current_token->type))
			return (make_position(current_token,
					current_token->content.expanded_word));
		if ((!line || *line == '\0') && current_token->next)
		{
			current_token = current_token->next;
			line = current_token->content.expanded_word;
		}
		if (*line && isnotifs(*line, ifs))
			return (make_position(current_token, line));
		if (*line)
			line++;
	}
	return (make_position(NULL, NULL));
}

static t_response	lex_malloc_failure(t_token *to_free)
{
	token_free_list(to_free);
	return (make_response(MALLOC_FAILURE, NULL));
}

static t_response	create_an_empty_token(void)
{
	char	*empty_string;
	t_token	*token;

	empty_string = ft_strdup("");
	if (empty_string == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	token = token_create_raw(WORD_TYPE, empty_string, 0, true);
	if (token == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	return (make_response(SUCCESS, token));
}

t_response	lex_tokens_into_new_tokens(t_token *token, const char *ifs)
{
	t_chunk		beginning;
	t_token		**last_token;
	t_token		*head;
	t_pos		ifs_pos;
	t_pos		curr_pos;

	head = NULL;
	last_token = &head;
	ifs_pos = make_position(token, token->content.expanded_word);
	curr_pos = find_first_char(ifs_pos, ifs);
	while (curr_pos.line)
	{
		ifs_pos = find_first_ifs(curr_pos, ifs);
		beginning = new_chunk(curr_pos.line, curr_pos.current_token->next,
				find_distance_to_next_ifs(curr_pos, ifs));
		*last_token = token_create_from_many(beginning);
		if (*last_token == NULL)
			return (lex_malloc_failure(head));
		last_token = &((*last_token)->next);
		curr_pos = find_first_char(ifs_pos, ifs);
	}
	if (head == NULL && has_quotes(token))
		return (create_an_empty_token());
	return (make_response(SUCCESS, head));
}
