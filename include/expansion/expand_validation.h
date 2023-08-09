/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_validation.h                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/13 09:45:29 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/13 09:45:37 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_VALIDATION_H
# define EXPAND_VALIDATION_H

# include <stdbool.h>
# include "lexer/token.h"

int		isifs(int c, const char *ifs);
int		isnotifs(int c, const char *ifs);
bool	is_quote(t_token_type type);
int		iswordindquote(int c);

#endif
