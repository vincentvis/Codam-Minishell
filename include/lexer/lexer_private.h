/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_private.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/12 16:58:00 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/05/12 16:58:44 by jowytrzy      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_PRIVATE_H
# define LEXER_PRIVATE_H

# include "utils/status.h"

t_response	lex_and_make_tokens(const char *line, const t_state state,
				const t_transition *const transitions);

#endif
