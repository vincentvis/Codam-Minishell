/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_nbrlen_unsigned.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/12/13 15:31:08 by vvissche      #+#    #+#                 */
/*   Updated: 2022/01/02 18:16:20 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

/*
** This is about 4,5 tunes faster than the 'simple' while loop approach
** Counts for both signed/unsigned and for any base..
*/

uint_fast8_t	ft_nbrlen_unsigned(unsigned long value)
{
	if (value < 10)
		return (1);
	if (value < 100)
		return (2);
	if (value < 1000)
		return (3);
	if (value < 10000)
		return (4);
	if (value < 100000)
		return (5);
	if (value < 1000000)
		return (6);
	if (value < 10000000)
		return (7);
	if (value < 100000000)
		return (8);
	if (value < 1000000000)
		return (9);
	if (value < 10000000000)
		return (10);
	if (value < 100000000000)
		return (11);
	if (value < 1000000000000)
		return (12);
	return (12 + ft_nbrlen_unsigned(value / 1000000000000));
}

uint_fast8_t	ft_nbrlen_unsigned_base16(unsigned long value)
{
	if (value < 16)
		return (1);
	if (value < 256)
		return (2);
	if (value < 4096)
		return (3);
	if (value < 65536)
		return (4);
	if (value < 1048576)
		return (5);
	if (value < 16777216)
		return (6);
	if (value < 268435456)
		return (7);
	if (value < 4294967296)
		return (8);
	if (value < 68719476736)
		return (9);
	if (value < 1099511627776)
		return (10);
	if (value < 17592186044416)
		return (11);
	if (value < 281474976710656)
		return (12);
	return (12 + ft_nbrlen_unsigned_base16(value / 281474976710656));
}

uint_fast8_t	ft_nbrlen_unsigned_base8(unsigned long value)
{
	if (value < 8)
		return (1);
	if (value < 64)
		return (2);
	if (value < 512)
		return (3);
	if (value < 4096)
		return (4);
	if (value < 32768)
		return (5);
	if (value < 262144)
		return (6);
	if (value < 2097152)
		return (7);
	if (value < 16777216)
		return (8);
	if (value < 134217728)
		return (9);
	if (value < 1073741824)
		return (10);
	if (value < 8589934592)
		return (11);
	if (value < 68719476736)
		return (12);
	return (12 + ft_nbrlen_unsigned_base8(value / 68719476736));
}

uint_fast8_t	ft_nbrlen_unsigned_base2(unsigned long value)
{
	if (value < 2)
		return (1);
	if (value < 4)
		return (2);
	if (value < 8)
		return (3);
	if (value < 16)
		return (4);
	if (value < 32)
		return (5);
	if (value < 64)
		return (6);
	if (value < 128)
		return (7);
	if (value < 256)
		return (8);
	if (value < 512)
		return (9);
	if (value < 1024)
		return (10);
	if (value < 2048)
		return (11);
	if (value < 4096)
		return (12);
	return (12 + ft_nbrlen_unsigned_base2(value / 4096));
}

/*
** this seems to be about 10% faster than assigning it to a variable after
** each step..
** if base is known, use one of the optimized versions
** they are on average about 6% faster
*/

uint_fast8_t	ft_nbrlen_unsigned_base(unsigned long value, unsigned long base)
{
	if (base == 16)
		return (ft_nbrlen_unsigned_base16(value));
	if (base == 8)
		return (ft_nbrlen_unsigned_base8(value));
	if (base == 2)
		return (ft_nbrlen_unsigned_base2(value));
	if (value < base)
		return (1);
	if (value < base * base)
		return (2);
	if (value < base * base * base)
		return (3);
	if (value < base * base * base * base)
		return (4);
	if (value < base * base * base * base * base)
		return (5);
	if (value < base * base * base * base * base * base)
		return (6);
	if (value < base * base * base * base * base * base * base)
		return (7);
	if (value < base * base * base * base * base * base * base * base)
		return (8);
	return (8 + ft_nbrlen_unsigned_base(value / (base * base * base
				* base * base * base * base * base), base));
}
