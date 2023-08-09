/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_bigint_double.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/15 10:32:54 by vincent       #+#    #+#                 */
/*   Updated: 2021/03/23 16:11:54 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_bigint.h"

/*
** \brief	Takes apart the double into the parts needed to convert to string
**
** Casting to to unsigned long through tricking C and then being able to do
** bit manipulation to extract sign/exponent/fraction parts
** Converts the fraction part to a string to loop through bits
** -- could have probably done this by masking and shifting (1 << i) >> (i - 1)
*/

void	ft_bigint_set_dbl_struct(t_dbl *info, double dbl)
{
	ft_memset(info, 0, sizeof(t_dbl));
	info->bits = *(unsigned long *)&dbl;
	info->exponent_value = ((info->bits & DBL_EXP_MASK) >> 52) - DBL_BIAS;
	info->fraction_value = (1ULL << 52) | (info->bits & DBL_FRAC_MASK);
	info->sign = -((info->bits & DBL_SIGN_MASK) >> 63);
	info->right = (info->bits & DBL_FRAC_MASK) << 11;
	if (info->exponent_value >= 0 && info->exponent_value <= 52)
		info->left = info->fraction_value >> (52 - info->exponent_value);
	if (info->exponent_value > 0 && info->exponent_value <= 52)
		info->right = (info->fraction_value << (info->exponent_value + 12));
	if (info->exponent_value > 52)
		info->left = info->fraction_value;
	if (info->exponent_value > 52)
		info->right = 0;
	if (info->exponent_value < 0)
		info->right = info->fraction_value << 11;
	if (info->right != 0)
		info->frac_len = ft_nbrlen_unsigned_base2(info->right);
	if (info->right != 0)
		ft_nbrtostr_n_base2(info->fract_bin + (64 - info->frac_len),
			info->right, info->frac_len);
	if (info->exponent_value == -DBL_BIAS)
		info->fract_bin[0] = '0';
	if (info->exponent_value == -DBL_BIAS)
		info->exponent_value = -1022;
}

/*
** \brief	Exception for when float is zero
*/

t_bigint	*ft_bigint_double_zero(t_bigint *bigint)
{
	if (bigint->sign == -1)
	{
		bigint->str[BIGINT_DECIMAL_POS - 2] = '-';
		bigint->startpos = BIGINT_DECIMAL_POS - 2;
	}
	return (bigint);
}

/*
** \brief	adds infinity and nan exceptions
*/

t_bigint	*ft_bigint_inf_or_nan(t_bigint *bigint, t_dbl *info)
{
	bigint->startpos = 1;
	bigint->len_digit = 0;
	bigint->len_dbl = 0;
	bigint->len_decimal = 0;
	bigint->endpos = 4;
	if ((info->bits << 12) == 0)
	{
		bigint->is_inf = 1;
		ft_memcpy(bigint->str, "-inf\0-INF", 10);
		if (bigint->sign == -1)
		{
			bigint->startpos = 0;
			bigint->endpos = 4;
		}
		return (bigint);
	}
	bigint->is_nan = 1;
	ft_memcpy(bigint->str, " nan\0 NAN", 10);
	return (bigint);
}

/*
** \brief	handle the "right" (decimals) side of the bigint
*/

static t_bigint	*ft_bigint_double_wstr_right(t_bigint *bigint, t_dbl *info,
											int bit, unsigned long div)
{
	t_bigint		decimal;

	decimal = *ft_bigint_from_unsigned(&decimal, 1);
	while (bit + info->exponent_value > 60)
	{
		ft_bigint_div(&decimal, 1ULL << 60);
		info->exponent_value -= 60;
	}
	ft_bigint_div(&decimal, 1ULL << (bit + info->exponent_value));
	ft_bigint_add(bigint, &decimal, bigint);
	bit++;
	while (bit < 53)
	{
		if (info->fract_bin[bit] == '1')
		{
			ft_bigint_div(&decimal, 1ULL << div);
			ft_bigint_add(bigint, &decimal, bigint);
			div = 0;
		}
		div++;
		bit++;
	}
	return (bigint);
}

/*
** \brief	handle the "left" (digits) side of the bigint
*/

t_bigint	*ft_bigint_double_with_str(t_bigint *bigint, t_dbl *info)
{
	int				bit;

	if (info->left > 0)
	{
		bigint = ft_bigint_from_unsigned(NULL, info->left);
		if (info->exponent_value > 52)
			bigint = ft_bigint_l_shift(bigint, info->exponent_value - 52);
	}
	if (info->right > 0)
	{
		bit = ft_memchr(info->fract_bin, '1', 53) - (void *)info->fract_bin;
		if (info->exponent_value > 0)
			info->exponent_value = 1;
		else
			info->exponent_value *= -1;
		bigint = ft_bigint_double_wstr_right(bigint, info, bit, 1UL);
	}
	if (info->sign == -1)
	{
		bigint->sign = -1;
		bigint->startpos--;
		bigint->str[bigint->startpos] = '-';
	}
	return (bigint);
}
