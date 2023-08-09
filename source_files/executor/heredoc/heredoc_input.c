/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_input.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/18 17:35:42 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 18:05:16 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "lexer/token.h"
#include "utils/status.h"
#include "utils/signals.h"
#include "utils/readline.h"
#include "expansion/expand_functions.h"
#include "expansion/transition_tables.h"
#include "lexer/lexer_private.h"

static int	write_line_to_heredoc(int fd_write, char *line)
{
	if (write(fd_write, line, ft_strlen(line)) == -1
		|| write(fd_write, "\n", 1) == -1)
	{
		free(line);
		close(fd_write);
		return (-1);
	}
	return (0);
}

static char	*expand_heredoc_line(char *line)
{
	const t_response	lex_token_content = lex_and_make_tokens(line,
			START_STATE, transition_table_for_heredoc_content());
	t_response			process_inner_tokens;
	char				*expanded_line;

	if (lex_token_content.status != SUCCESS)
	{
		free(line);
		return (NULL);
	}
	expanded_line = NULL;
	process_inner_tokens = expand_var_tokens_in_list(lex_token_content.data);
	if (process_inner_tokens.status == SUCCESS)
		expanded_line = join_token_list(lex_token_content.data).data;
	token_free_list(lex_token_content.data);
	free(line);
	return (expanded_line);
}

int	get_heredoc_input_in_child(const char *eof, int fd_write,
	bool should_expand)
{
	char			*line;
	const size_t	eof_len = ft_strlen(eof) + 1;

	line = NULL;
	while (get_heredoc_line(&line))
	{
		if (line == NULL || (ft_memcmp(line, eof, eof_len) == 0))
			break ;
		if (should_expand && *line)
			line = expand_heredoc_line(line);
		if (line == NULL)
			break ;
		if (write_line_to_heredoc(fd_write, line) == -1)
			return (WRITE_FAILURE);
	}
	set_signals_default();
	if (line)
	{
		free (line);
		close(fd_write);
	}
	return (SUCCESS);
}
