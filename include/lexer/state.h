/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   state.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/06/20 15:57:16 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/05/19 12:24:02 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_H
# define STATE_H

typedef enum e_state {
	START_STATE,
	IN_QUOTE_STATE,
	IN_DQUOTE_STATE,
	internal_VALID_TOKEN_END_STATE,
	DIGIT_STATE,
	WORD_STATE,
	DLESS_STATE,
	DGREAT_STATE,
	EXITCODE_STATE,
	VAR_ARG_STATE,
	PIPE_STATE,
	LESS_STATE,
	GREAT_STATE,
	TILDE_STATE,
	VAR_START_STATE,
	IN_VAR_STATE,
	QUOTE_END,
	DQUOTE_END,
	INTERNAL_END_STATE,
}		t_state;

typedef struct s_transition
{
	t_state	current;
	t_state	next;
	int		(*func)(int);
}	t_transition;

#endif
