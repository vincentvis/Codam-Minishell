/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ifs.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/07 14:48:10 by vvissche      #+#    #+#                 */
/*   Updated: 2022/03/15 16:50:04 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef IFS_H
# define IFS_H

# define DEFAULT_IFS " \t\n"

enum e_ifs_actions {
	IFS_GET,
	IFS_TRY_UPDATE,
	IFS_RESET
};

char	*get_ifs(enum e_ifs_actions action);

#endif
