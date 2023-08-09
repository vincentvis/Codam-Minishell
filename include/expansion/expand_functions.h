/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_functions.h                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/13 09:41:00 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/13 09:58:23 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_FUNCTIONS_H
# define EXPAND_FUNCTIONS_H

# include "utils/status.h"
# include "lexer/token.h"

typedef struct s_pos {
	t_token		*current_token;
	char		*line;
}				t_pos;

typedef struct s_chunks
{
	const char		*start;
	const t_token	*tokens;
	size_t			total_length;
}	t_chunk;

t_response	expand_word_type(t_token *token);
t_response	expand_var_type(t_token *token);
t_response	expand_dquote_type(t_token *token);
t_response	expand_simple_quote_type(t_token *token);
t_response	expand_tilde_type(t_token *token);
t_response	expand_exitcode_type(t_token *token);
t_response	expand_var_tokens_in_list(t_token *token);
t_response	expand_dollarsign_type(t_token *token);
t_response	join_token_list(t_token *start);

size_t		find_distance_to_next_ifs(t_pos position, const char *ifs);
t_pos		find_first_ifs(t_pos position, const char *ifs);
t_pos		make_position(t_token *token, char *line);
bool		has_quotes(t_token *token);
t_token		*token_create_from_many(t_chunk info);
void		is_first_token_tilde(t_token *token);

#endif
