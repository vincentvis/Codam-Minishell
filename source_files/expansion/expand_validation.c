/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_validation.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/13 09:42:51 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 18:14:48 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "libft.h"
#include "lexer/token.h"

int	isifs(int c, const char *ifs)
{
	return (ft_strchr(ifs, c) != NULL);
}

int	isnotifs(int c, const char *ifs)
{
	return (!isifs(c, ifs));
}

bool	is_quote(t_token_type type)
{
	return (type == DQUOTE_TYPE || type == QUOTE_TYPE);
}

int	iswordindquote(int c)
{
	return (c != '"' && c != '$' && c != '\0');
}
