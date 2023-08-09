/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   transition_tables.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/12 16:45:21 by vvissche      #+#    #+#                 */
/*   Updated: 2022/06/09 10:06:07 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexer/state.h"
#include "lexer/validation.h"
#include "expansion/expand_functions.h"
#include "expansion/transition_tables.h"

const t_transition	*transition_table_for_word_type(void)
{
	static const t_transition	transitions[] = {
	{START_STATE, TILDE_STATE, &istilde},
	{START_STATE, VAR_START_STATE, &isdollarsign},
	{START_STATE, IN_QUOTE_STATE, &isquote},
	{START_STATE, IN_DQUOTE_STATE, &isdquote},
	{START_STATE, WORD_STATE, &iswordincompound},
	{START_STATE, START_STATE, &ft_isspace},
	{VAR_START_STATE, EXITCODE_STATE, &isquestionmark},
	{VAR_START_STATE, VAR_ARG_STATE, &isdigit_arg},
	{VAR_START_STATE, IN_VAR_STATE, &isvarstart},
	{IN_VAR_STATE, IN_VAR_STATE, &isinsidevar},
	{IN_QUOTE_STATE, IN_QUOTE_STATE, &isinsidequote},
	{IN_QUOTE_STATE, QUOTE_END, &isquote},
	{IN_DQUOTE_STATE, IN_DQUOTE_STATE, &isinsidedquote},
	{IN_DQUOTE_STATE, DQUOTE_END, &isdquote},
	{WORD_STATE, WORD_STATE, &iswordincompound},
	{INTERNAL_END_STATE, INTERNAL_END_STATE, NULL}};

	return (transitions);
}

const t_transition	*transition_table_for_here_eof_type(void)
{
	static const t_transition	transitions[] = {
	{START_STATE, IN_QUOTE_STATE, &isquote},
	{START_STATE, IN_DQUOTE_STATE, &isdquote},
	{START_STATE, WORD_STATE, &isword},
	{START_STATE, START_STATE, &ft_isspace},
	{IN_QUOTE_STATE, IN_QUOTE_STATE, &isinsidequote},
	{IN_QUOTE_STATE, QUOTE_END, &isquote},
	{IN_DQUOTE_STATE, IN_DQUOTE_STATE, &isinsidedquote},
	{IN_DQUOTE_STATE, DQUOTE_END, &isdquote},
	{WORD_STATE, WORD_STATE, &isword},
	{INTERNAL_END_STATE, INTERNAL_END_STATE, NULL}};

	return (transitions);
}

const t_transition	*transition_table_for_heredoc_content(void)
{
	static const t_transition	transitions[] = {
	{START_STATE, WORD_STATE, &ishereword},
	{START_STATE, VAR_START_STATE, &isdollarsign},
	{VAR_START_STATE, EXITCODE_STATE, &isquestionmark},
	{VAR_START_STATE, VAR_ARG_STATE, &isdigit_arg},
	{VAR_START_STATE, IN_VAR_STATE, &isinsidevar},
	{IN_VAR_STATE, IN_VAR_STATE, &isinsidevar},
	{WORD_STATE, WORD_STATE, &ishereword},
	{INTERNAL_END_STATE, INTERNAL_END_STATE, NULL}};

	return (transitions);
}

const t_transition	*get_transition_table_for_type(t_token_type type)
{
	const t_transition	*transitions[__INTERNAL_TYPE_END] = {
	[WORD_TYPE] = transition_table_for_word_type(),
	[HERE_EOF_TYPE] = transition_table_for_here_eof_type(),
	[HERE_EOF_EXP_TYPE] = transition_table_for_here_eof_type()
	};

	return (transitions[type]);
}

const t_jump	*get_jumptable_for_type(t_token_type type)
{
	static const t_jump	jumptable_word_type[__INTERNAL_TYPE_END] = {
	[WORD_TYPE] = &expand_word_type,
	[DOLLARSIGN_TYPE] = &expand_dollarsign_type,
	[VAR_TYPE] = &expand_var_type,
	[DQUOTE_TYPE] = &expand_dquote_type,
	[QUOTE_TYPE] = &expand_simple_quote_type,
	[TILDE_TYPE] = &expand_tilde_type,
	[EXITCODE_TYPE] = &expand_exitcode_type,
	};
	static const t_jump	jumptable_hereeof_type[__INTERNAL_TYPE_END] = {
	[WORD_TYPE] = &expand_word_type,
	[DQUOTE_TYPE] = &expand_simple_quote_type,
	[QUOTE_TYPE] = &expand_simple_quote_type,
	};

	if (type == HERE_EOF_TYPE || type == HERE_EOF_EXP_TYPE)
		return (jumptable_hereeof_type);
	return (jumptable_word_type);
}
