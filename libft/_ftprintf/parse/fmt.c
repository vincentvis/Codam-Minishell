/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fmt.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/29 15:31:55 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 15:52:02 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

/*
** Function Parse Flags
**
** Description
** Checks in g_ipf_map if it's a valid flag (char's of flags are set to 1 to 5)
** As long as it's a valid flag, use OR operator to set the flag
** Use of OR ensures that the same flag found multiple times has no effect
** Increments index of reading in *fmt to skip all flag chars
**
** Parameters
** [t_pfdata]	{pfdata}	Pointer of main data object
** [site_t]		{i}			current position in format string
**
** Return value
** [size_t]					index with amount of flag chars added
*/

size_t	ipf__parse_flags(t_pfdata **pfdata, size_t i)
{
	const uint_fast8_t	*char_map = ipf_char_map();

	while (char_map[(int)(*pfdata)->fmt[i]] >= 1
			&& char_map[(int)(*pfdata)->fmt[i]] <= 16)
	{
		(*pfdata)->tail->flags.value |= char_map[(int)(*pfdata)->fmt[i]];
		i++;
	}
	return (i);
}

/*
** Function Parse Width
**
** Description
** Uses ft_atoi to determine width and progresses `i` past numbers
** Or gets an argument from the va_list to set the width
**
** Parameters
** [t_pfdata]	{pfdata}	Pointer of main data object
** [site_t]		{start}		current position in format string
**
** Return
** [size_t]					index with amount of width chars added
*/

size_t	ipf__parse_width(t_pfdata **pfdata, size_t i)
{
	int	width;

	width = 0;
	if (ft_isdigit((*pfdata)->fmt[i]))
	{
		(*pfdata)->tail->width.value = ft_atoi(&(*pfdata)->fmt[i]);
		i += ft_atoi_chars(&(*pfdata)->fmt[i]);
		if ((*pfdata)->tail->flags.value & FLG_ZERO)
			(*pfdata)->tail->width.str[0] = '0';
	}
	if ((*pfdata)->fmt[i] == '*')
	{
		width = va_arg((*pfdata)->ap, int);
		i++;
		if (width < 0)
		{
			(*pfdata)->tail->flags.value |= FLG_MINU;
			width *= -1;
		}
		(*pfdata)->tail->width.value = width;
		if ((*pfdata)->tail->flags.value & FLG_ZERO)
			(*pfdata)->tail->width.str[0] = '0';
	}
	return (i);
}

/*
** Function Parse Precision
**
** Description
** Uses ft_atoi to determine with and progresses `i` past numbers
** Negative precision via * is ignored
** Negative precision hardcoded in string (%.-5) is undefined but implemented
** the same way as printf behaved on my testing machine
**
** Parameters
** [t_pfdata]	{pfdata}	Pointer of main data object
** [site_t]		{start}		current position in format string
**
** Return
** [size_t]					index with amount of precision chars added
*/

size_t	ipf__parse_precision(t_pfdata **pfdata, size_t i)
{
	int		precision;

	precision = 0;
	if ((*pfdata)->fmt[i] == '.')
	{
		i++;
		if (ft_isdigit((*pfdata)->fmt[i]) || (*pfdata)->fmt[i] == '-')
		{
			precision = ft_atoi(&(*pfdata)->fmt[i]);
			i += ft_atoi_chars(&(*pfdata)->fmt[i]);
			(*pfdata)->tail->precision.value = precision;
			if (precision < 0)
				ipf__parse_precision_undef(pfdata, precision);
		}
		else if ((*pfdata)->fmt[i] == '*')
		{
			precision = va_arg((*pfdata)->ap, int);
			i++;
			if (precision < 0)
				return (i);
			(*pfdata)->tail->precision.value = precision;
		}
		(*pfdata)->tail->precision.str[0] = '0';
	}
	return (i);
}

/*
** Function Parse length
**
** Description
** Checks for length modifiers (h/hh and l/ll)
**
** Parameters
** [t_pfdata]	{pfdata}	Pointer of main data object
** [site_t]		{start}		current position in format string
**
** Return
** [size_t]					index with amount of length chars added
*/

size_t	ipf__parse_length(t_pfdata **pfdata, size_t i)
{
	if ((*pfdata)->fmt[i] == 'h')
	{
		i++;
		(*pfdata)->tail->length = LNG_SHRT;
		if ((*pfdata)->fmt[i] == 'h')
		{
			i++;
			(*pfdata)->tail->length = LNG_CHAR;
		}
	}
	else if ((*pfdata)->fmt[i] == 'l')
	{
		i++;
		(*pfdata)->tail->length = LNG_LONG;
		if ((*pfdata)->fmt[i] == 'l')
		{
			i++;
			(*pfdata)->tail->length = LNG_LLNG;
		}
	}
	return (i);
}

/*
** Function Parse type
**
** Description
** Uses global map to check if type exists
** If type does not exist print just a char (this is what printf seems to do)
**
** Parameters
** [t_pfdata]	{pfdata}	Pointer of main data object
** [site_t]		{start}		current position in format string
**
** Return
** [size_t]					index with amount of type chars added
*/

size_t	ipf__parse_type(t_pfdata **pfdata, size_t i)
{
	const uint_fast8_t	*char_map = ipf_char_map();
	const t_jmp			*jumptable = ipf_jump_format();

	if (char_map[(int)(*pfdata)->fmt[i]] > 16)
	{
		(*pfdata)->tail->type = (*pfdata)->fmt[i];
		(*pfdata)->error = jumptable[(int)(*pfdata)->fmt[i] - JMP_OFFST](pfdata,
				(*pfdata)->tail);
		i++;
		if ((*pfdata)->error < 0)
			return ((*pfdata)->error);
	}
	else
	{
		i += ipf__fmt_type_invalid(pfdata, i);
	}
	return (i);
}
