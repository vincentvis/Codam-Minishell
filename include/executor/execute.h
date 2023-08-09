/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/08 21:24:04 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/05/15 12:07:32 by jowytrzy      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "parser/parser.h"
# include "utils/status.h"

t_response	execute(const t_ast_node *tree);

#endif
