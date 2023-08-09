/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/06/20 15:53:45 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/09 09:43:26 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "../lexer/state.h"
# include <stdbool.h>
# include <stddef.h>

typedef union u_value
{
	int			digit;
	char		*expanded_word;
	const char	*raw_word;
}	t_value;

typedef enum e_token_type
{
	WORD_TYPE,
	QUOTE_TYPE,
	DQUOTE_TYPE,
	HERE_EOF_TYPE,
	HERE_EOF_EXP_TYPE,
	VAR_TYPE,
	DOLLARSIGN_TYPE,
	EXITCODE_TYPE,
	TILDE_TYPE,
	IO_NUMBER_TYPE,
	PIPE_TYPE,
	_REDIRECTION,
	LESS_TYPE,
	DLESS_TYPE,
	GREAT_TYPE,
	DGREAT_TYPE,
	__INTERNAL_TYPE_END
}	t_token_type;

typedef enum e_token_source
{
	LEXER,
	PARSER,
	EXPANSION,
	UPDATE_TREE,
	HEREDOC,
	UNKNOWN,
}	t_token_source;

typedef struct s_token
{
	t_token_type	type;
	t_value			content;
	size_t			length;
	bool			allocated;
	t_token_source	source;
	struct s_token	*next;
}	t_token;

t_token_type	token_determine_type(const t_state state);
t_token			*token_create_raw(const t_token_type type,
					const char *raw_word, const size_t length,
					const bool allocated);
t_token			*token_create_ionumber_type(int number);
void			token_review_lexed_types(t_token *tokens);
void			token_free_list(t_token *head);

#endif
