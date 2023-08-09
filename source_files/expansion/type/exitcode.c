/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exitcode.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/13 09:29:14 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/13 09:37:22 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "lexer/token.h"
#include "utils/status.h"
#include "utils/exitcode.h"

t_response	expand_exitcode_type(t_token *token)
{
	int		exitcode;
	char	*string;
	int		sign;
	size_t	length;

	exitcode = get_last_exit_code();
	sign = 0;
	if (exitcode < 0)
	{
		sign = 1;
		exitcode *= -1;
	}
	length = ft_nbrtoastr(&string, exitcode, sign);
	if (string == NULL)
		return (make_response(MALLOC_FAILURE, NULL));
	if (token->allocated)
		free(token->content.expanded_word);
	token->content.expanded_word = string;
	token->length = length;
	token->allocated = true;
	token->type = WORD_TYPE;
	return (make_response(SUCCESS, token));
}
