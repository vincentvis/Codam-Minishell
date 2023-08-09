/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   interactive.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/24 14:27:36 by vvissche      #+#    #+#                 */
/*   Updated: 2022/05/26 13:51:03 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERACTIVE_H
# define INTERACTIVE_H

# include <stdbool.h>

bool	is_interactive(void);
bool	is_not_interactive(void);
bool	set_interactive(void);
bool	set_not_interactive(void);

#endif
