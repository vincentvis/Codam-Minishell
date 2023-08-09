/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal_handlers.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/18 17:06:22 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 17:43:02 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>
#include "ft_printf.h"
#include "utils/readline.h"
#include "utils/exitcode.h"

void	signal_int(int status)
{
	if (status == SIGINT)
	{
		signal(SIGINT, SIG_IGN);
		ft_printf("\033[1G%s%s  \n", get_prompt(), rl_line_buffer);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		signal(SIGINT, signal_int);
	}
}

int	handle_ctrld(void)
{
	ft_printf("\033[1F" "%sexit\n", get_prompt());
	return (get_last_exit_code());
}

void	signal_int_heredoc(int status)
{
	ft_printf("\033[1G" HEREDOC_PROMPT "%s  \n", rl_line_buffer);
	rl_on_new_line();
	rl_replace_line("", 0);
	signal(SIGINT, SIG_DFL);
	kill(getpid(), status);
	(void)status;
}
