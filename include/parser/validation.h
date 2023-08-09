/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   validation.h                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/07/23 10:34:31 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/07 17:45:43 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATION_H
# define VALIDATION_H

# include <stdbool.h>
# include "lexer/token.h"

bool	is_command_suffix(t_token_type type);
bool	is_redirection(t_token_type type);
bool	is_word(t_token_type type);
bool	is_pipe(t_token_type type);
bool	is_io_number(t_token_type type);

#endif
