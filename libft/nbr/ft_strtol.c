/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strtol.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/12/05 10:32:34 by vvissche      #+#    #+#                 */
/*   Updated: 2020/05/08 10:19:28 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
	The strtol() function converts the string in str to a long value.
	The conversion is done according to the given base, which must be between 2
	and 36 inclusive, or be the special value 0.

	The string may begin with an arbitrary amount of white space (as determined
	by isspace(3)) followed by a single optional `+' or `-' sign.  If base is
	zero or 16, the string may then include a ``0x'' prefix, and the number
	will be read in base 16; otherwise, a zero base is taken as 10 (decimal)
	unless the next character is `0', in which case it is taken as 8 (octal).

	The remainder of the string is converted to a long value in the obvious
	manner, stopping at the first character which is not a valid digit in the
	given base. (In bases above 10, the letter `A' in either upper or lower case
	represents 10, `B' represents 11, and so forth, with `Z' representing 35.)

	If endptr is not NULL, strtol() stores the address of the first invalid
	character in *endptr. If there were no digits at all, however, strtol()
	stores the original value of str in *endptr. (Thus, if *str is not `\0'
	but **endptr is `\0' on return, the entire string was valid.)
*/
long	ft_strtol(const char *str, char **endptr, int base)
{
	return ((long)ft_strtoll(str, endptr, base));
}
