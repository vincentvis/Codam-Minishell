/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_ifs_find_pos.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/08 16:38:41 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 18:13:57 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "lexer/token.h"
#include "expansion/expand_functions.h"
#include "expansion/expand_validation.h"

static char	*find_first_ifs_in_line(char *line, const char *ifs)
{
	while (*line)
	{
		if (ft_strchr(ifs, *line))
			return (line);
		line++;
	}
	return (NULL);
}

static void	goto_next_token_if_it_exists(t_token **current_token, char **line)
{
	while ((*line == NULL || **line == '\0') && (*current_token)->next)
	{
		(*current_token) = (*current_token)->next;
		*line = (*current_token)->content.expanded_word;
	}
}

t_pos	find_first_ifs(t_pos position, const char *ifs)
{
	t_token	*current_token;
	char	*line;

	current_token = position.current_token;
	line = position.line;
	while (current_token && (current_token->next || (line && *line)))
	{
		goto_next_token_if_it_exists(&current_token, &line);
		if (current_token && is_quote(current_token->type))
		{
			if (current_token->next == NULL)
				return (make_position(current_token, &(current_token->content
							.expanded_word[current_token->length])));
			current_token = current_token->next;
			line = current_token->content.expanded_word;
			continue ;
		}
		line = find_first_ifs_in_line(line, ifs);
		if (line)
			return (make_position(current_token, line));
	}
	return (make_position(current_token, &(current_token->content
				.expanded_word[current_token->length])));
}

size_t	find_distance_to_next_ifs(t_pos position, const char *ifs)
{
	size_t	count;

	count = 0;
	while (position.current_token && (position.current_token->next
			|| (position.line && *position.line)))
	{
		goto_next_token_if_it_exists(&position.current_token, &position.line);
		if (position.current_token && is_quote(position.current_token->type))
		{
			count += position.current_token->length;
			if (position.current_token->next == NULL)
				return (count);
			position.current_token = position.current_token->next;
			position.line = position.current_token->content.expanded_word;
			continue ;
		}
		if (*position.line && isifs(*position.line, ifs))
			return (count);
		if (*position.line)
		{
			position.line++;
			count++;
		}
	}
	return (count);
}
