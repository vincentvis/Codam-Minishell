/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_nbrtostr_base16.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 08:57:57 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 14:09:22 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_nbr.h"

int	ft_nbrtoastr_base16(char **dst, unsigned long long nbr,
							int_fast8_t uplow)
{
	return (ft_nbrtoastr_n_base16(dst, nbr,
			ft_nbrlen_unsigned_base16(nbr), uplow));
}

int	ft_nbrtoastr_n_base16(char **dst, unsigned long long nbr,
		uint_fast8_t nbrlen, int_fast8_t uplow)
{
	*dst = malloc(sizeof(**dst) * (nbrlen + 1));
	if (*dst == NULL)
		return (-1);
	return (ft_nbrtostr_n_base16(*dst, nbr, nbrlen, uplow));
}

int	ft_nbrtostr_base16(char *dst, unsigned long long nbr, int_fast8_t uplow)
{
	return (ft_nbrtostr_n_base16(dst, nbr,
			ft_nbrlen_unsigned_base16(nbr), uplow));
}

/*
	`uplow` should either be 'x' or 'X' (120 or 88)
	'x' = 01111000b
	'X' = 01011000b
	mask= 00100000b
	so we can mask for the difference, shift and multiply
*/

int	ft_nbrtostr_n_base16(char *dst, unsigned long long nbr,
		uint_fast8_t nbrlen, int_fast8_t uplow)
{
	uint64_t		division;
	const short		*base_16 = (short*)
		&(get_base16()[!((uplow & 32) >> 5) * 512]);
	const char		*base_36 = &get_base36()[!((uplow & 32) >> 5) * 36];

	dst[nbrlen] = 0;
	dst[0] = '0';
	division = nbr;
	dst += nbrlen;
	while (division >= 256)
	{
		dst -= 2;
		division /= 256;
		*(short *)dst = base_16[nbr - (256 * division)];
		nbr = division;
	}
	while (division > 0)
	{
		dst--;
		division /= 16;
		*dst = base_36[(nbr - (16 * division))];
		nbr = division;
	}
	return (nbrlen);
}
