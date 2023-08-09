/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_bigint_from.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/12 08:15:26 by vincent       #+#    #+#                 */
/*   Updated: 2021/03/23 16:12:26 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_bigint.h"

/*
** \brief	Create a bigint from an unsigned long
**
** Adds at str[BIGINT_DECIMAL_POS] the '.' back in to override the '\0' placed
** by ft_nbrtostr_n.
*/

t_bigint	*ft_bigint_from_unsigned(t_bigint *bigint,
										unsigned long long num)
{
	if (bigint == NULL)
		bigint = ft_bigint_create();
	else
		ft_bigint_set_default(bigint);
	if (bigint == NULL)
		return (NULL);
	bigint->len_digit = ft_nbrlen_unsigned(num);
	bigint->startpos = BIGINT_DECIMAL_POS - bigint->len_digit;
	ft_nbrtostr_n(bigint->str + bigint->startpos,
		num, bigint->len_digit, 0);
	bigint->str[BIGINT_DECIMAL_POS] = '.';
	return (bigint);
}

/*
** \brief	Create a bigint from a long long number
**
** Adds at str[BIGINT_DECIMAL_POS] the '.' back in to override the '\0' placed
** by ft_nbrtostr_n.
*/

t_bigint	*ft_bigint_from_signed(t_bigint *bigint, long long num)
{
	if (bigint == NULL)
		bigint = ft_bigint_create();
	else
		ft_bigint_set_default(bigint);
	if (bigint == NULL)
		return (NULL);
	if (num < 0)
	{
		bigint->sign = 1;
		num *= -1;
	}
	bigint->len_digit = ft_nbrlen_unsigned(num);
	bigint->startpos = BIGINT_DECIMAL_POS - bigint->len_digit;
	ft_nbrtostr_n(bigint->str + bigint->startpos - bigint->sign,
		num, bigint->len_digit, bigint->sign);
	bigint->str[BIGINT_DECIMAL_POS] = '.';
	bigint->num_str = bigint->str + bigint->startpos;
	return (bigint);
}

/*
** \brief	Everthing that didn't fit in the ft_bigint_from_str function
*/

static void	ft_bigint_from_str_after_decimal(t_bigint *bigint, char *str)
{
	bigint->len_decimal = 0;
	while (ft_isdigit(str[bigint->len_decimal])
		&& bigint->len_decimal < BIGINT_DECIMAL_MAX)
	{
		bigint->len_decimal++;
	}
	bigint->endpos = BIGINT_DECIMAL_POS + bigint->len_decimal;
	ft_memcpy(bigint->str + BIGINT_DECIMAL_POS + 1, str, bigint->len_decimal);
	bigint->len_dbl = bigint->len_digit + bigint->len_decimal + 1;
}

/*
** \brief	Create a bigint from a string input
*/

t_bigint	*ft_bigint_from_str(t_bigint *bigint, char *str)
{
	if (bigint == NULL)
		bigint = ft_bigint_create();
	else
		ft_bigint_set_default(bigint);
	if (bigint == NULL)
		return (NULL);
	while (*str == '0' || ft_isspace(*str))
		str++;
	if (*str == '-')
		bigint->sign = 1;
	bigint->len_digit = 0;
	if (str[bigint->len_digit + bigint->sign] != '0')
	{
		while (ft_isdigit(str[bigint->len_digit + bigint->sign]))
			bigint->len_digit++;
		bigint->startpos = BIGINT_DECIMAL_POS - bigint->len_digit;
		ft_memcpy(bigint->str + bigint->startpos - bigint->sign,
			str, bigint->len_digit + bigint->sign);
		bigint->num_str = bigint->str + bigint->startpos;
	}
	bigint->len_dbl = bigint->len_digit + 2;
	if (str[bigint->len_digit + bigint->sign] == '.')
		ft_bigint_from_str_after_decimal(bigint,
			&str[bigint->len_digit + 1 + bigint->sign]);
	return (bigint);
}

/*
** \brief	Start converting a double value to a string representation
**
** Sets the values into a double information struct.
** Then it defines some bounds for either 'simple' processing,
** 'difficult' processing, denormals, nan/inf
** and goes into the respective branch to do it's work
*/

t_bigint	*ft_bigint_from_double(t_bigint *bigint, double dbl)
{
	t_dbl	info;

	if (bigint == NULL)
		bigint = ft_bigint_create();
	else
		ft_bigint_set_default(bigint);
	if (bigint == NULL)
		return (NULL);
	ft_bigint_set_dbl_struct(&info, dbl);
	bigint->sign = info.sign;
	if ((info.bits << 1) == 0)
		return (ft_bigint_double_zero(bigint));
	if (info.exponent_value == DBL_INF)
		return (ft_bigint_inf_or_nan(bigint, &info));
	return (ft_bigint_double_with_str(bigint, &info));
}
