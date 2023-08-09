/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ifs.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/07 14:48:10 by vvissche      #+#    #+#                 */
/*   Updated: 2022/03/15 16:51:05 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "env/she_env.h"
#include "utils/ifs.h"

char	*get_ifs(enum e_ifs_actions action)
{
	static char	*ifs = NULL;
	t_response	response;

	if (ifs == NULL || action == IFS_TRY_UPDATE)
	{
		response = sheenv_get_value_by_key("IFS");
		if (response.status == SUCCESS)
			ifs = response.data;
		else
			ifs = DEFAULT_IFS;
	}
	if (action == IFS_RESET)
		ifs = DEFAULT_IFS;
	return (ifs);
}
