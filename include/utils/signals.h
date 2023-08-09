/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/05 10:47:05 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 16:26:38 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

void	signal_int(int status);
void	signal_int_heredoc(int status);
int		handle_ctrld(void);
void	set_signals_default(void);
void	set_signals_for_child(void);
void	set_signals_for_parent(void);
void	set_signals_for_heredoc(void);
void	set_signals_for_heredoc_parent(void);

#endif
