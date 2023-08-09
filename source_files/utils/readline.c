/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   readline.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/05 10:47:05 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 18:20:03 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "ft_printf.h"
#include "utils/minishell_colors.h"
#include "utils/readline.h"
#include "utils/signals.h"

char	*get_prompt(void)
{
	return (PROMPT_NO_PRINT_START \
		CYAN \
		PROMPT_NO_PRINT_END \
		BEFORE_SAUROPOD \
		PROMPT_NO_PRINT_START \
		COLOR_OFF \
		PROMPT_NO_PRINT_END \
		SAUROPOD \
		EN_SPACE \
		" " \
		PROMPT_NO_PRINT_START \
		"\033[5G" \
		PROMPT_NO_PRINT_END);
}

char	*get_line(char **line)
{
	set_signals_default();
	if (line && *line)
	{
		free(*line);
		*line = NULL;
	}
	*line = readline(get_prompt());
	if (line && *line && **line)
		add_history(*line);
	return (*line);
}

char	*get_heredoc_line(char **line)
{
	set_signals_for_heredoc();
	if (line && *line)
	{
		free(*line);
		*line = NULL;
	}
	*line = readline(HEREDOC_PROMPT);
	return (*line);
}
