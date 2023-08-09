/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_nbrtostr_base2.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 08:57:57 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 14:13:06 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_nbr.h"
#include <stdlib.h>

int	ft_nbrtoastr_base2(char **dst, unsigned long long nbr)
{
	uint_fast8_t		nbrlen;

	nbrlen = ft_nbrlen_unsigned_base2(nbr);
	*dst = malloc(sizeof(**dst) * (nbrlen + 1));
	if (*dst == NULL)
		return (-1);
	return (ft_nbrtostr_n_base2(*dst, nbr, nbrlen));
}

int	ft_nbrtostr_base2(char *dst, unsigned long long nbr)
{
	return (ft_nbrtostr_n_base2(dst, nbr, ft_nbrlen_unsigned_base2(nbr)));
}

int	ft_nbrtostr_n_base2(char *dst, unsigned long long nbr,
							uint_fast8_t nbrlen)
{
	const short			*base2 = (short*)"00011011";
	unsigned long long	division;

	dst[nbrlen] = 0;
	dst[0] = '0';
	division = nbr;
	dst += nbrlen;
	while (division >= 4)
	{
		dst -= 2;
		division /= 4;
		*(short *)dst = base2[nbr - (4 * division)];
		nbr = division;
	}
	while (division > 0)
	{
		dst--;
		division /= 2;
		*dst = (nbr - (2 * division)) + '0';
		nbr = division;
	}
	return (nbrlen);
}
