/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   validation_words.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/06 09:16:27 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/26 18:12:01 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	isword(int c)
{
	return (!ft_isspace(c) && !ft_strchr("<>|'\"", c) && c != '\0');
}

int	isquote(int c)
{
	return (c == '\'');
}

int	isdquote(int c)
{
	return (c == '"');
}

int	isinsidequote(int c)
{
	return (!isquote(c) && c != '\0');
}

int	isinsidedquote(int c)
{
	return (!isdquote(c) && c != '\0');
}
