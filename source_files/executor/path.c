/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   path.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/05/29 11:39:01 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/07 18:09:37 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "ft_printf.h"
#include "utils/status.h"
#include "env/she_env.h"

static t_response	find_existing_path(char *const *prefixes,
						const char *link, const char *suffix)
{
	char	*try;

	while (prefixes && *prefixes)
	{
		ft_asprintf(&try, "%s%s%s", *prefixes, link, suffix);
		if (try == NULL)
			return (make_response(MALLOC_FAILURE, NULL));
		if (access(try, F_OK) == 0)
			return (make_response(SUCCESS, try));
		free(try);
		prefixes++;
	}
	ft_asprintf(&try, "%s", suffix);
	if (try == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	if (access(try, F_OK) == 0)
		return (make_response(SUCCESS, try));
	free(try);
	return (make_response(SUCCESS, NULL));
}

static t_response	split_path(const char *path_value, char delim)
{
	char	**pre_paths;

	if (path_value == NULL)
		return (make_response(SUCCESS, NULL));
	pre_paths = ft_split(path_value, delim);
	if (pre_paths == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	return (make_response(SUCCESS, pre_paths));
}

t_response	response_strdup(const char *string)
{
	char *const	copy = ft_strdup(string);

	if (copy == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	return (make_response(SUCCESS, copy));
}

t_response	get_command_path(const char *cmd)
{
	const char	*path_value = sheenv_get_value_by_key("PATH").data;
	char		**pre_paths;
	t_response	response;

	if (ft_strchr(cmd, '/'))
		return (response_strdup(cmd));
	response = split_path(path_value, ':');
	if (response.status != SUCCESS)
		return (response);
	pre_paths = response.data;
	response = find_existing_path(pre_paths, "/", cmd);
	free_char_pointer_array(pre_paths);
	return (response);
}
