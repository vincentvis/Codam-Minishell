/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   validation_compound_words.c                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/18 17:44:44 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/18 17:44:46 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	istilde(int c)
{
	return (c == '~');
}

int	ishereword(int c)
{
	return (c != '$' && c != '\0');
}

int	iswordincompound(int c)
{
	return (!ft_isspace(c) && !ft_strchr("$<>|'\"", c) && c != '\0');
}
