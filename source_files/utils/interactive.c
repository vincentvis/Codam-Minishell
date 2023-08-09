/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   interactive.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/24 14:24:45 by vvissche      #+#    #+#                 */
/*   Updated: 2022/06/07 17:38:19 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

bool	*interactive(void)
{
	static bool	interactive = true;

	return (&interactive);
}

bool	is_interactive(void)
{
	return (*interactive() == true);
}

bool	is_not_interactive(void)
{
	return ((*interactive()) == false);
}

bool	set_interactive(void)
{
	*interactive() = true;
	return (true);
}

bool	set_not_interactive(void)
{
	*interactive() = false;
	return (false);
}
