/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_bigint.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/10 15:59:32 by vincent       #+#    #+#                 */
/*   Updated: 2021/11/28 13:21:14 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_bigint.h"

void	*ft_bigint_destroy(t_bigint *bigint)
{
	free(bigint);
	return (NULL);
}

/*
** len_digit	length of the integer part (not rounded)
** len_dbl		length of double, so what is before and after comma
** 			 	if there is nothing after comma, we add 1 pos for zero
**           	zo 0.0 becomes len = 3
*/

void	ft_bigint_set_default(t_bigint *bigint)
{
	ft_memset(bigint->str, '0', BIGINT_STR_MAX);
	bigint->str[BIGINT_DECIMAL_POS] = '.';
	bigint->str[BIGINT_STR_MAX] = 0;
	bigint->sign = 0;
	bigint->len_digit = 1;
	bigint->len_decimal = 0;
	bigint->len_dbl = 3;
	bigint->startpos = BIGINT_DECIMAL_POS - bigint->len_digit;
	bigint->endpos = BIGINT_DECIMAL_POS + bigint->len_decimal;
	bigint->num_str = bigint->str + bigint->startpos;
	bigint->is_inf = 0;
	bigint->is_nan = 0;
}

/*
** \brief	Rounds a bigint to `round_to` positions behind decimal point
**
** This only makes sense if there is a decimal part..
**
** Uses strategy: round to nearest, ties to even
*/

t_bigint	*ft_bigint_round(t_bigint *bigint, int round_to)
{
	int			pos;
	t_bigint	add;

	if (bigint->is_nan || bigint->is_inf)
		return (bigint);
	pos = round_to + BIGINT_DECIMAL_POS + 1;
	if (pos >= BIGINT_STR_MAX)
		return (bigint);
	if (round_to == 0 && bigint->str[pos] == '5'
		&& (bigint->str[BIGINT_DECIMAL_POS - 1] & 1) == 0)
		bigint->str[BIGINT_DECIMAL_POS - 1]--;
	if (bigint->str[pos] >= '5' && bigint->str[pos] <= '9')
	{
		add = *ft_bigint_create();
		add.str[pos] = '5';
		ft_bigint_add(bigint, &add, bigint);
	}
	bigint->endpos = pos;
	bigint->len_decimal = bigint->endpos - BIGINT_DECIMAL_POS;
	bigint->len_dbl = bigint->len_digit + 1 + bigint->len_decimal;
	ft_memset(bigint->str + pos, '0', BIGINT_STR_MAX - pos - 2);
	return (bigint);
}

/*
** \brief	Initializes a big int with zero as value
*/

t_bigint	*ft_bigint_create(void)
{
	t_bigint	*bigint;

	bigint = malloc(sizeof(*bigint));
	if (bigint == NULL)
		return (NULL);
	ft_bigint_set_default(bigint);
	return (bigint);
}
