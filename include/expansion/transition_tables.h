/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   transition_tables.h                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/13 09:24:27 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/13 09:58:29 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRANSITION_TABLES_H
# define TRANSITION_TABLES_H

typedef t_response	(*t_jump)(t_token *token);

const t_transition	*transition_table_for_word_type(void)
					__attribute__((always_inline));
const t_transition	*transition_table_for_here_eof_type(void)
					__attribute__((always_inline));
const t_transition	*get_transition_table_for_type(t_token_type type)
					__attribute__((always_inline));
const t_transition	*transition_table_for_heredoc_content(void)
					__attribute__((always_inline));

const t_jump		*get_jumptable_for_type(t_token_type type);

#endif
