/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_bigint_add.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/12 08:15:10 by vincent       #+#    #+#                 */
/*   Updated: 2021/03/23 16:11:15 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_bigint.h"

/*
** \brief	Finish up the addition part
*/

static void	ft_bigint_add_wrapup(t_bigint *dest, int i, int d1, int d2)
{
	if (dest->str[i + 1] == '.')
	{
		dest->str[i + 1] = '1';
		dest->str[i] = '-';
		i--;
	}
	dest->startpos = i + 1;
	if (dest->str[dest->startpos] == '0')
		dest->startpos++;
	dest->len_digit = BIGINT_DECIMAL_POS - dest->startpos;
	if (d2 > d1)
		d1 = d2;
	dest->len_decimal = d1;
	while (dest->str[dest->len_decimal + BIGINT_DECIMAL_POS] == '0')
		dest->len_decimal--;
	dest->endpos = (dest->len_decimal * (_Bool)dest->len_decimal)
		+ BIGINT_DECIMAL_POS;
	dest->len_dbl = dest->len_digit + dest->len_decimal + 1;
	if (dest->len_decimal == 0)
		dest->len_dbl++;
}

/*
** \brief	Add two bigint values together
**
** Value is saved in destination, if dest is NULL, value is saved in the left
** value. This way you can either change the first value, or store the result
** in a new t_bigint.
**
** Addition is from Right to Left.
** From 'max', 2 is subtracted, 1 for decimal point and 1 for the carry
*/

void	ft_bigint_add(t_bigint *left, t_bigint *right, t_bigint *dest)
{
	int	i;
	int	result;
	int	carry;
	int	max;

	if (dest == NULL)
		dest = left;
	carry = 0;
	i = left->len_decimal * (left->len_decimal > right->len_decimal)
		+ right->len_decimal * (1 - (left->len_decimal > right->len_decimal))
		+ BIGINT_DECIMAL_POS;
	max = BIGINT_DECIMAL_POS - 2 - (left->len_digit * (left->len_digit
				> right->len_digit) + right->len_digit * (1
				- (left->len_digit > right->len_digit)));
	while (i > max)
	{
		if (left->str[i] == '.')
			i--;
		result = left->str[i] + right->str[i] + carry - 96;
		carry = result / 10;
		result %= 10;
		dest->str[i] = result + 48;
		i--;
	}
	ft_bigint_add_wrapup(dest, i, left->len_decimal, right->len_decimal);
}

/*
** \brief	Adds a string to a bigint
*/

void	ft_bigint_add_str_to_bigint(t_bigint *left, char *str,
										t_bigint *dest)
{
	t_bigint	right;

	ft_bigint_from_str(&right, str);
	ft_bigint_add(left, &right, dest);
}

/*
** \brief	Adds two strings together as bigints and returns a bigint
*/

t_bigint	*ft_bigint_add_str_to_str(char *left_str, char *right_str)
{
	t_bigint	left;
	t_bigint	right;
	t_bigint	*dest;

	dest = ft_bigint_create();
	ft_bigint_from_str(&left, left_str);
	ft_bigint_from_str(&right, right_str);
	ft_bigint_add(&left, &right, dest);
	return (dest);
}

/*
** \brief	Adds a long value to bigint
*/

void	ft_bigint_add_long_to_bigint(t_bigint *bigint, long add,
										t_bigint *dest)
{
	t_bigint	right;

	right = *ft_bigint_from_signed(&right, add);
	ft_bigint_add(bigint, &right, dest);
}
