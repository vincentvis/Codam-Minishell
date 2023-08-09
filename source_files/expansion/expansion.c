/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expansion.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/13 09:42:33 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/09 10:07:53 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "expansion/expansion.h"
#include "expansion/expand_functions.h"
#include "expansion/transition_tables.h"
#include "utils/ifs.h"
#include "utils/status.h"
#include "lexer/state.h"
#include "lexer/lexer_private.h"
#include "lexer/token.h"

static t_response	call_functions_for_type(t_token *token,
	t_token_type parent_type)
{
	const t_jump		*jumptable = get_jumptable_for_type(parent_type);
	t_token				*start;
	t_response			expand_response;

	start = token;
	while (token)
	{
		if (jumptable[token->type])
		{
			expand_response = jumptable[token->type](token);
			if (expand_response.status != SUCCESS)
				return (expand_response);
		}
		token = token->next;
	}
	return (make_response(SUCCESS, start));
}

static t_response	expand_on_ifs(t_token *list)
{
	const char	*ifs = get_ifs(IFS_TRY_UPDATE);
	t_response	tokens;

	if (list == NULL)
		return (make_response(SUCCESS, NULL));
	tokens = lex_tokens_into_new_tokens(list, ifs);
	return (tokens);
}

static void	fix_types(t_token *token, t_token_type type)
{
	if ((type == HERE_EOF_TYPE
			|| type == HERE_EOF_EXP_TYPE
			|| type == DOLLARSIGN_TYPE)
		&& token->type == WORD_TYPE)
		token->type = type;
}

t_response	expand_tc_fail(t_response ret, void *to_free,
	void (*cleanup_function)(void *ptr))
{
	cleanup_function(to_free);
	return (ret);
}

t_response	expand_token_content(t_token *token)
{
	char *const	token_content = ft_substr(token->content.raw_word, 0,
			token->length);
	t_response	lex_token_content;
	t_response	processed_tokens;
	t_response	expanded_ifs;

	if (token_content == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	lex_token_content = lex_and_make_tokens(token_content, START_STATE,
			get_transition_table_for_type(token->type));
	if (lex_token_content.status != SUCCESS)
		return (expand_tc_fail(lex_token_content, token_content, &free));
	is_first_token_tilde(lex_token_content.data);
	processed_tokens = call_functions_for_type(lex_token_content.data,
			token->type);
	free(token_content);
	if (processed_tokens.status != SUCCESS)
		return (expand_tc_fail(processed_tokens, lex_token_content.data,
				(void (*)(void *))token_free_list));
	expanded_ifs = expand_on_ifs(processed_tokens.data);
	token_free_list(processed_tokens.data);
	fix_types(expanded_ifs.data, token->type);
	return (make_response(SUCCESS, expanded_ifs.data));
}
