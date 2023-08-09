/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_bigint_mul.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/13 18:39:10 by vincent       #+#    #+#                 */
/*   Updated: 2021/03/23 16:12:47 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_bigint.h"

/*
** \brief	Multiplies some bigint by some unsigned number
**
** Multiplication is from right to left
*/

void	ft_bigint_mul(t_bigint *bigint, unsigned long long mul)
{
	unsigned long long	carry;
	unsigned long long	num;
	int					pos;

	pos = bigint->endpos;
	carry = 0ULL;
	while ((pos >= bigint->startpos || carry != 0) && pos >= 0)
	{
		if (bigint->str[pos] == '.')
			pos--;
		num = ((bigint->str[pos] - 48) * mul) + carry;
		carry = num / 10;
		num %= 10;
		bigint->str[pos] = num + 48;
		pos--;
	}
	bigint->startpos = pos + 1;
	while (bigint->str[bigint->endpos] == '0')
		bigint->endpos--;
	bigint->len_digit = BIGINT_DECIMAL_POS - bigint->startpos;
	bigint->len_decimal = bigint->endpos - BIGINT_DECIMAL_POS;
	bigint->len_dbl = bigint->len_digit + bigint->len_decimal + 1;
	if (bigint->len_decimal == 0)
		bigint->len_dbl++;
}

/*
** \brief	Left shift some bigint string by N places
**
** shifting goes by max 60 at a time because at higher values you get ulong
** overflow if you multiply by 9. In other words: ((1 << 61) * 9) > ULONG_MAX
*/

t_bigint	*ft_bigint_l_shift(t_bigint *bigint, int n)
{
	if (bigint == NULL)
		bigint = ft_bigint_create();
	while (n >= 60)
	{
		ft_bigint_mul(bigint, 0x1000000000000000);
		n -= 60;
	}
	if (n > 0)
		ft_bigint_mul(bigint, 1UL << n);
	return (bigint);
}

/*
** \brief	Turn an unsigned long into a string and shift it left
**
** Just a shorthand function for the ft_bigint_l_shift function
** but allows things like; ft_bigint_l_shift_long(1ULL, 128) to get (1 << 128)
*/

t_bigint	*ft_bigint_l_shift_long(unsigned long long num, int n)
{
	t_bigint		*result;

	result = ft_bigint_from_unsigned(NULL, num);
	return (ft_bigint_l_shift(result, n));
}

/*
** \brief	multiplies by 10 'n' times
**
** since we know that multipyling by 10 is just moving everything one position
** to the right, we can just do that n many times.
** memmove the whole string N places to the right. set N places in the beginning
** to '0'. Move whats now on spot of decimal 1 place to te right and place
** decimal at correct point. Update start/endpos.
*/

void	ft_bigint_mul_10(t_bigint *bigint, int n)
{
	ft_memmove(bigint->str + bigint->startpos - n,
		bigint->str + bigint->startpos,
		bigint->endpos - bigint->startpos + 1);
	ft_memset(bigint->str + bigint->endpos, '0', n);
	ft_memmove(bigint->str + BIGINT_DECIMAL_POS - n,
		bigint->str + BIGINT_DECIMAL_POS - n + 1, n);
	bigint->str[BIGINT_DECIMAL_POS] = '.';
	bigint->startpos -= n;
	bigint->endpos -= n;
}
