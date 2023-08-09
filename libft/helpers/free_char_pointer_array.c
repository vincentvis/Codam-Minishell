/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free_char_pointer_array.c                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jwytrzyszczewska <jwytrzyszczewska@stud      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/17 00:28:32 by jwytrzyszcz   #+#    #+#                 */
/*   Updated: 2022/02/17 00:58:41 by jwytrzyszcz   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	free_char_pointer_array(char **array)
{
	int	i;

	i = 0;
	if (array)
	{
		while ((array)[i])
		{
			free((array)[i]);
			i++;
		}
		free(array);
	}
}
