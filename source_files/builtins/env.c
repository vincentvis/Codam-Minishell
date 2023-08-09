/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/11 11:49:33 by vvissche      #+#    #+#                 */
/*   Updated: 2022/06/07 17:54:59 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "env/she_env.h"

int	builtin_env(char *const *args)
{
	(void)args;
	return (sheenv_print(SKIP_EMPTY_VALUES));
}
