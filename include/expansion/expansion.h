/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expansion.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/13 09:44:24 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 09:58:19 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "utils/status.h"
# include "parser/parser.h"
# include "lexer/token.h"

t_response	expand_tree(t_ast_node **head);
t_response	expand_token_content(t_token *token);
t_response	lex_tokens_into_new_tokens(t_token *token, const char *ifs);

#endif
