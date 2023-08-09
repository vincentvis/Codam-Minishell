/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   validation_var.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/26 17:45:24 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/26 18:11:33 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	isdollarsign(int c)
{
	return (c == '$');
}

int	isvarstart(int c)
{
	return (c == '_' || ft_isalpha(c));
}

int	isinsidevar(int c)
{
	return (!ft_isspace(c) && (c == '_' || ft_isalnum(c)) && c != '\0');
}

int	isdigit_arg(int c)
{
	return (ft_isdigit(c));
}
