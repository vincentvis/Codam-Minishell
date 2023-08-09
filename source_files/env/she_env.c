/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   she_env.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/06 09:16:27 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/26 18:15:10 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "env/she_env.h"

// Order of strings in env are not defined
// https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap08.html
// > There is no meaning associated with the order of strings in the environment

t_env_base	*sheenv(void)
{
	static t_env_base		env;

	return (&env);
}
