/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_nbrtostr.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 08:57:57 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 13:43:32 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_nbr.h"
#include <stdlib.h>

/*
	IF `sign` == 0
		number is positive
	IF `sign` == 1
		number is negative
*/

int	ft_nbrtoastr(char **dst, unsigned long long nbr, int_fast8_t sign)
{
	return (ft_nbrtoastr_n(dst, nbr, ft_nbrlen_unsigned(nbr), sign));
}

int	ft_nbrtoastr_n(char **dst, unsigned long long nbr, uint_fast8_t nbrlen,
						int_fast8_t sign)
{
	*dst = malloc(sizeof(**dst) * (nbrlen + 1 + sign));
	if (*dst == NULL)
		return (-1);
	return (ft_nbrtostr_n(*dst, nbr, nbrlen, sign));
}

int	ft_nbrtostr(char *dst, unsigned long long nbr, int_fast8_t sign)
{
	return (ft_nbrtostr_n(dst, nbr, ft_nbrlen_unsigned(nbr), sign));
}

/**
 * dst[0] = '0' - ((sign & 1) * 3);
 * set first char to a zero, since it won't go into any of the loops if nbr=0
 * if negative, set it to '-' (which is ascii 45, 3 lower than '0'(48))
 * anding is probably not neccecary in "clean" use, but better on misuse
 * of sign (which _should_ be 1 or 0.. but could be any uint_fast8_t)
 */

int	ft_nbrtostr_n(char *dst, unsigned long long nbr, uint_fast8_t nbrlen,
						int_fast8_t sign)
{
	const short			*base10 = get_base10_short();
	unsigned long long	division;

	dst[0] = '0' - ((sign & 1) * 3);
	dst[nbrlen + sign] = 0;
	division = nbr;
	if (nbr != 0)
		dst += nbrlen + sign;
	while (division >= 100)
	{
		dst -= 2;
		division = division / 100;
		*(short *)dst = base10[nbr - (100 * division)];
		nbr = division;
	}
	while (division > 0)
	{
		dst--;
		division = division / 10;
		*dst = (char)(nbr - (10 * division)) + '0';
		nbr = division;
	}
	return (nbrlen);
}
