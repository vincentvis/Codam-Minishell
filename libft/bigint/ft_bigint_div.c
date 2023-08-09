/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_bigint_div.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/13 12:42:59 by vincent       #+#    #+#                 */
/*   Updated: 2021/03/23 16:11:32 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_bigint.h"

/*
** \brief	divides some bigint by some unsigned number
**
** Division is from left to right
*/

void	ft_bigint_div(t_bigint *bigint, unsigned long long div)
{
	unsigned long long	remainder;
	unsigned long long	num;
	unsigned long long	intemediary_result;
	int					pos;

	pos = bigint->startpos;
	remainder = 0ULL;
	while ((pos <= bigint->endpos || remainder != 0) && pos < BIGINT_STR_MAX)
	{
		if (bigint->str[pos] == '.')
			pos++;
		num = bigint->str[pos] - 48 + remainder;
		intemediary_result = num / div;
		remainder = ((intemediary_result * div) - num) * -10;
		bigint->str[pos] = intemediary_result + 48;
		pos++;
	}
	bigint->endpos = pos;
	while (bigint->str[bigint->startpos] == '0')
		bigint->startpos++;
	bigint->len_digit = BIGINT_DECIMAL_POS - bigint->startpos;
	bigint->len_decimal = bigint->endpos - BIGINT_DECIMAL_POS - 1;
	bigint->len_dbl = bigint->len_digit + bigint->len_decimal + 1;
}

/*
** \brief	Right shift some bigint string by N places
**
** shifting goes by max 60 at a time because at higher values you get ulong
** overflow if you multiply by 9. In other words: ((1 << 61) * 9) > ULONG_MAX
*/

t_bigint	*ft_bigint_r_shift(t_bigint *bigint, int n)
{
	if (bigint == NULL)
		bigint = ft_bigint_create();
	while (n >= 60)
	{
		ft_bigint_div(bigint, 0x1000000000000000);
		n -= 60;
	}
	ft_bigint_div(bigint, 1UL << n);
	return (bigint);
}

/*
** \brief	divides by 10 'n' times
**
** since we know that dividing by 10 is just moving everything one position to
** the right, we can just do that n many times.
** memmove the whole string N places to the right. set N places in the beginning
** to '0'. Move whats now on spot of decimal 1 place to te right and place
** decimal at correct point. Update start/endpos.
*/

void	ft_bigint_div_10(t_bigint *bigint, int n)
{
	ft_memmove(bigint->str + bigint->startpos + n,
		bigint->str + bigint->startpos,
		bigint->endpos - bigint->startpos + 1);
	ft_memset(bigint->str + bigint->startpos, '0', n);
	ft_memmove(bigint->str + BIGINT_DECIMAL_POS + 1,
		bigint->str + BIGINT_DECIMAL_POS, n);
	bigint->str[BIGINT_DECIMAL_POS] = '.';
	bigint->startpos += n;
	bigint->endpos += n;
}
