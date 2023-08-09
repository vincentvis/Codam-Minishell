/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   readline.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/05 10:47:05 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/26 14:17:22 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef READLINE_H
# define READLINE_H

# define HEREDOC_PROMPT "> "

char	*get_prompt(void) __attribute__((always_inline));
char	*get_line(char **line);
char	*get_heredoc_line(char **line);

#endif
