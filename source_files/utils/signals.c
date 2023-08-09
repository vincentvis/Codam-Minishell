/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/05 10:47:05 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 17:43:42 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "utils/signals.h"

void	set_signals_default(void)
{
	signal(SIGINT, signal_int);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signals_for_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	set_signals_for_parent(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signals_for_heredoc(void)
{
	signal(SIGINT, signal_int_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signals_for_heredoc_parent(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
