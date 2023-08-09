/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_nbrtostr_base.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/06 08:57:57 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 13:51:02 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_nbr.h"
#include <stdlib.h>

int	ft_nbrtostr_base(char *dst, unsigned long long nbr, uint_fast8_t base)
{
	const char			*base_str = get_base36();
	uint_fast8_t		nbrlen;
	unsigned long long	division;

	if (base < 2 || base > 36)
		return (-1);
	if (base == 2)
		return (ft_nbrtostr_n_base2(dst, nbr, ft_nbrlen_unsigned_base2(nbr)));
	nbrlen = ft_nbrlen_unsigned_base(nbr, base);
	if (base == 10)
		return (ft_nbrtostr_n(dst, nbr, nbrlen, 0));
	if (base == 16)
		return (ft_nbrtostr_n_base16(dst, nbr, nbrlen, 'x'));
	dst[nbrlen] = 0;
	dst[0] = '0';
	dst += nbrlen;
	division = nbr;
	while (division)
	{
		dst--;
		division = division / base;
		*dst = base_str[(nbr - (base * division))];
		nbr = division;
	}
	return (nbrlen);
}

static char	*alloc_nbr(size_t nbrlen, char **dst, char **str)
{
	*dst = malloc(sizeof(**dst) * (nbrlen + 1));
	if (*dst == NULL)
		return (NULL);
	*str = *dst;
	(*str)[nbrlen] = 0;
	return (*str);
}

int	ft_nbrtoastr_base(char **dst, unsigned long long nbr, uint_fast8_t base)
{
	const char			*base_str = get_base36();
	size_t				nbrlen;
	unsigned long long	division;
	char				*str;

	if (base < 2 || base > 36)
		return (-1);
	nbrlen = ft_nbrlen_unsigned_base(nbr, base);
	if (alloc_nbr(nbrlen, dst, &str) == NULL)
		return (-1);
	if (base == 10)
		return (ft_nbrtostr_n(str, nbr, nbrlen, 0));
	if (base == 16)
		return (ft_nbrtostr_n_base16(str, nbr, nbrlen, 'x'));
	str[0] = '0';
	str += nbrlen;
	division = nbr;
	while (division)
	{
		str--;
		division = division / base;
		*str = base_str[(nbr - (base * division))];
		nbr = division;
	}
	return (nbrlen);
}
