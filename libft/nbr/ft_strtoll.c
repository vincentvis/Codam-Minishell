/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strtoll.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/12/05 10:06:05 by vvissche      #+#    #+#                 */
/*   Updated: 2022/05/11 17:16:11 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_nbr.h"
#include <limits.h>
#include <errno.h>

#define HAS_OVERFLOW 1
#define IS_NEGATIVE 1
#define IS_POSITIVE 0

static const t_basemap			g_basemap[127] = {
['0'] = {.base = 2, .value = 0},
['1'] = {.base = 2, .value = 1},
['2'] = {.base = 3, .value = 2},
['3'] = {.base = 4, .value = 3},
['4'] = {.base = 5, .value = 4},
['5'] = {.base = 6, .value = 5},
['6'] = {.base = 7, .value = 6},
['7'] = {.base = 8, .value = 7},
['8'] = {.base = 9, .value = 8},
['9'] = {.base = 10, .value = 9},
['A'] = {.base = 11, .value = 10},
['a'] = {.base = 11, .value = 10},
['B'] = {.base = 12, .value = 11},
['b'] = {.base = 12, .value = 11},
['C'] = {.base = 13, .value = 12},
['c'] = {.base = 13, .value = 12},
['D'] = {.base = 14, .value = 13},
['d'] = {.base = 14, .value = 13},
['E'] = {.base = 15, .value = 14},
['e'] = {.base = 15, .value = 14},
['F'] = {.base = 16, .value = 15},
['f'] = {.base = 16, .value = 15},
['G'] = {.base = 17, .value = 16},
['g'] = {.base = 17, .value = 16},
['H'] = {.base = 18, .value = 17},
['h'] = {.base = 18, .value = 17},
['I'] = {.base = 19, .value = 18},
['i'] = {.base = 19, .value = 18},
['J'] = {.base = 20, .value = 19},
['j'] = {.base = 20, .value = 19},
['K'] = {.base = 21, .value = 20},
['k'] = {.base = 21, .value = 20},
['L'] = {.base = 22, .value = 21},
['l'] = {.base = 22, .value = 21},
['M'] = {.base = 23, .value = 22},
['m'] = {.base = 23, .value = 22},
['N'] = {.base = 24, .value = 23},
['n'] = {.base = 24, .value = 23},
['O'] = {.base = 25, .value = 24},
['o'] = {.base = 25, .value = 24},
['P'] = {.base = 26, .value = 25},
['p'] = {.base = 26, .value = 25},
['Q'] = {.base = 27, .value = 26},
['q'] = {.base = 27, .value = 26},
['R'] = {.base = 28, .value = 27},
['r'] = {.base = 28, .value = 27},
['S'] = {.base = 29, .value = 28},
['s'] = {.base = 29, .value = 28},
['T'] = {.base = 30, .value = 29},
['t'] = {.base = 30, .value = 29},
['U'] = {.base = 31, .value = 30},
['u'] = {.base = 31, .value = 30},
['V'] = {.base = 32, .value = 31},
['v'] = {.base = 32, .value = 31},
['W'] = {.base = 33, .value = 32},
['w'] = {.base = 33, .value = 32},
['X'] = {.base = 34, .value = 33},
['x'] = {.base = 34, .value = 33},
['Y'] = {.base = 35, .value = 34},
['y'] = {.base = 35, .value = 34},
['Z'] = {.base = 36, .value = 35},
['z'] = {.base = 36, .value = 35},
};

static const unsigned long long	g_max[35] = {
	LONG_MAX / 2, LONG_MAX / 3, LONG_MAX / 4, LONG_MAX / 5, LONG_MAX / 6,
	LONG_MAX / 7, LONG_MAX / 8, LONG_MAX / 9, LONG_MAX / 10, LONG_MAX / 11,
	LONG_MAX / 12, LONG_MAX / 13, LONG_MAX / 14, LONG_MAX / 15, LONG_MAX / 16,
	LONG_MAX / 17, LONG_MAX / 18, LONG_MAX / 19, LONG_MAX / 20, LONG_MAX / 21,
	LONG_MAX / 22, LONG_MAX / 23, LONG_MAX / 24, LONG_MAX / 25, LONG_MAX / 26,
	LONG_MAX / 27, LONG_MAX / 28, LONG_MAX / 29, LONG_MAX / 30, LONG_MAX / 31,
	LONG_MAX / 32, LONG_MAX / 33, LONG_MAX / 34, LONG_MAX / 35, LONG_MAX / 36,
};

static const unsigned long long	g_rem[35] = {
	LONG_MAX % 2, LONG_MAX % 3, LONG_MAX % 4, LONG_MAX % 5, LONG_MAX % 6,
	LONG_MAX % 7, LONG_MAX % 8, LONG_MAX % 9, LONG_MAX % 10, LONG_MAX % 11,
	LONG_MAX % 12, LONG_MAX % 13, LONG_MAX % 14, LONG_MAX % 15, LONG_MAX % 16,
	LONG_MAX % 17, LONG_MAX % 18, LONG_MAX % 19, LONG_MAX % 20, LONG_MAX % 21,
	LONG_MAX % 22, LONG_MAX % 23, LONG_MAX % 24, LONG_MAX % 25, LONG_MAX % 26,
	LONG_MAX % 27, LONG_MAX % 28, LONG_MAX % 29, LONG_MAX % 30, LONG_MAX % 31,
	LONG_MAX % 32, LONG_MAX % 33, LONG_MAX % 34, LONG_MAX % 35, LONG_MAX % 36,
};

static uint_fast8_t	ft_strtoll_setup(int *base, unsigned long long *sign,
									const char **str, char **endptr)
{
	while (ft_isspace(**str))
		(*str)++;
	if (**str == '-')
		*sign = IS_NEGATIVE;
	if (**str == '-' || **str == '+')
		(*str)++;
	if (*base < 0 || *base == 1 || *base > 36)
		return (0);
	if (**str == '0')
	{
		if ((*base == 0 || *base == 16)
			&& ((*str)[1] == 'x' || (*str)[1] == 'X'))
		{
			(*str) += 2;
			*base = 16;
		}
		else if (*base == 0)
			*base = 8;
	}
	else if (*base == 0)
		*base = 10;
	if (endptr)
		*endptr = (char *)*str;
	return (1);
}

static char	*ft_strtoll_start(char **endptr,
						const char *str,
						unsigned long long *n,
						int base)
{
	if (endptr)
		*endptr = (char *)str;
	n[3] = IS_POSITIVE;
	n[4] = 0;
	n[0] = 0;
	n[1] = g_max[base - 2];
	n[2] = g_rem[base - 2];
	return ((char *)str);
}

/*
** If no conversion has taken place and there is an endptr
** we still need to set it properly
** there is an edgecase, for base 0/16 and the first 2 chars are 0 and x/X
** but the rest is not hexadecimal
** then we return the pointer to the x/X and return 0
*/

static long long	noconv(char **endptr, char *save)
{
	if (endptr)
	{
		if (*endptr - save >= 2
			&& ((*endptr)[-1] == 'x' || (*endptr)[-1] == 'X')
			&& (*endptr)[-2] == '0')
		{
			*endptr = &((*endptr)[-1]);
		}
		else
		{
			*endptr = save;
		}
	}
	return (0LL);
}

static long long	ft_strtoll_return(unsigned long long n[5])
{
	if (n[4] == HAS_OVERFLOW)
	{
		errno = ERANGE;
		if (n[3] == IS_NEGATIVE)
			return (LONG_MIN);
		return (LONG_MAX);
	}
	if (n[3] == IS_NEGATIVE)
		return (-((long long)n[0]));
	return ((long long)n[0]);
}

/*
** n[0] = number we are trying to get from the string
** n[1] = g_max = max value for base
** n[2] = g_rem = modulus for base
** n[3] = sign
** n[4] = overflow
*/

long long	ft_strtoll(const char *str,
				char **endptr,
				int base)
{
	char				*save;
	unsigned long long	n[5];
	t_basemap			num;

	save = ft_strtoll_start(endptr, str, n, base);
	if (!ft_strtoll_setup(&base, &n[3], &str, endptr))
		return (0LL);
	num = g_basemap[(int)*str];
	if (!(num.base <= base && num.base))
		return (noconv(endptr, save));
	while (num.base <= base && num.base)
	{
		if (n[0] > n[1] || (n[0] == n[1] && num.value > n[2]))
			n[4] = HAS_OVERFLOW;
		n[0] = num.value + (n[0] * base);
		str++;
		num = g_basemap[(int)*str];
	}
	if (endptr)
		*endptr = (char *)str;
	return (ft_strtoll_return(n));
}
