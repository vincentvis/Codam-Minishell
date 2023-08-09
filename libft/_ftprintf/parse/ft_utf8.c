/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_utf8.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/04 21:47:52 by vincent       #+#    #+#                 */
/*   Updated: 2022/04/12 15:49:40 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

/*
** Determine the width of an utf8 char
**
** Parameters
** [int]			{wchar}		the int to check the width from
*/

int	ipf__utf8_width(int wchar)
{
	if (wchar < 0)
		return (FT_UTF8_ERR);
	if (wchar <= 127)
		return (1);
	if (wchar <= 2047)
		return (2);
	if (wchar <= 65535)
		return (3);
	if (wchar <= FT_UTF8_MAX)
		return (4);
	return (FT_UTF8_ERR);
}

/*
** Get the string length of a int string
**
** Parameters
** [int*]			{wchar}		pointer to first int in int string
*/

int	ipf__utf8_strlen(int *wstr)
{
	int	total;
	int	width;
	int	i;

	total = 0;
	width = 0;
	i = 0;
	while (wstr[i] != L'\0')
	{
		width = ipf__utf8_width(wstr[i]);
		if (width == FT_UTF8_ERR)
			return (FT_UTF8_ERR);
		total += width;
		i++;
	}
	return (total);
}

/*
** Get the string length of a int string with a max number of chars that
** the end result can be long (precision as input)
**
** Parameters
** [int*]			{wchar}		pointer to first int in int string
** [int]			{max}		max lenght of char string
*/

int	ipf__utf8_strnlen(int *wstr, int max)
{
	int	total;
	int	width;
	int	i;

	i = 0;
	total = 0;
	width = 0;
	while (wstr[i] != L'\0' && total < max)
	{
		width = ipf__utf8_width(wstr[i]);
		if (width == FT_UTF8_ERR)
			return (FT_UTF8_ERR);
		total += width;
		if (total > max)
		{
			total -= width;
			break ;
		}
		i++;
	}
	return (total);
}

/*
** UTF8 conversion of int
**
** Description
** Writes 1 to 4 chars to a position pointed to by *dest
** Uses predefined bitmasks to make valid utf8 sequences
**
** Parameters
** [char*]			{dest}		destination of utf8 char
** [int]			{input}		the input int
** [int]			{width}		the utf8 width of the input
*/

void	ipf__utf8(char *dest, int input, int width)
{
	int	i;

	static const unsigned shift[8] = {0, 0, 6, 12, 18, 24, 30, 36};
	static const unsigned msk[8] = {0x3f, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc};
	if (width == 1)
	{
		dest[0] = input;
		return ;
	}
	dest[0] = (unsigned char)((input >> shift[width]) | msk[width]);
	i = 1;
	width--;
	while (width)
	{
		dest[i] = (unsigned char)(((input >> shift[width]) & 0x3f) | msk[1]);
		width--;
		i++;
	}
}

/*
** UTF8 conversion of string
**
** Description
** Calls ipf__utf8 for each int and writes the full char string to dest pointer
**
** Parameters
** [char*]			{dest}		destination of utf8 char string
** [int*]			{input}		int string as input
** [int]			{maxwidth}		the max width of the result
*/

int	ipf__utf8_str(char *dest, int *input, int maxwidth)
{
	int	i;
	int	width;

	i = 0;
	width = 0;
	while (input[i] != L'\0' && i < maxwidth)
	{
		width = ipf__utf8_width(input[i]);
		if (width == FT_UTF8_ERR)
			return (FT_UTF8_ERR);
		ipf__utf8(dest, input[i], width);
		dest += width;
		i++;
	}
	return (1);
}
