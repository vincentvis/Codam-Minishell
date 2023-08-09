/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fmt_get_arg.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/24 18:53:44 by vincent       #+#    #+#                 */
/*   Updated: 2022/04/12 15:49:18 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

void	ipf__get_arg_double(t_pfdata **pfdata, t_spec *fmt)
{
	fmt->arg.f = va_arg((*pfdata)->ap, double);
}

/*
** Get Argument for signed numbers
**
** Checks the length and gets correct casted argument with va_arg
*/

void	ipf__get_arg_signed(t_pfdata **pfdata, t_spec *fmt)
{
	if (fmt->length == LNG_LLNG)
		fmt->arg.lli = va_arg((*pfdata)->ap, long long);
	else if (fmt->length == LNG_LONG)
		fmt->arg.lli = va_arg((*pfdata)->ap, long);
	else if (fmt->length == LNG_SHRT)
		fmt->arg.lli = (short)va_arg((*pfdata)->ap, int);
	else if (fmt->length == LNG_CHAR)
		fmt->arg.lli = (char)va_arg((*pfdata)->ap, int);
	else
		fmt->arg.lli = va_arg((*pfdata)->ap, int);
}

/*
** Get Argument for unsigned numbers
**
** Checks the length and gets correct casted argument with va_arg
*/

void	ipf__get_arg_unsigned(t_pfdata **pfdata, t_spec *fmt)
{
	if (fmt->length == LNG_LLNG)
		fmt->arg.llu = va_arg((*pfdata)->ap, unsigned long long);
	else if (fmt->length == LNG_LONG)
		fmt->arg.llu = va_arg((*pfdata)->ap, unsigned long);
	else if (fmt->length == LNG_SIZT)
		fmt->arg.llu = va_arg((*pfdata)->ap, size_t);
	else if (fmt->length == LNG_SHRT)
		fmt->arg.llu = (unsigned short)va_arg((*pfdata)->ap, unsigned);
	else if (fmt->length == LNG_CHAR)
		fmt->arg.llu = (unsigned char)va_arg((*pfdata)->ap, unsigned);
	else
		fmt->arg.llu = va_arg((*pfdata)->ap, unsigned);
}

/*
** For when strings are NULL pointers
** Sets the string to a predefined constand char pointer with (null)
*/

static void	ipf_get_arg_string_null(t_spec *fmt, char length)
{
	if (length)
		fmt->arg.ls = STR_NULL_L;
	else
		fmt->arg.s = STR_NULL;
	fmt->strlen.value = 6;
	if (fmt->precision.str[0] != 0 && fmt->precision.value < 6)
		fmt->strlen.value = fmt->precision.value;
}

/*
** Get Argument for strings
**
** Checks the length and gets correct casted argument with va_arg
** If length == long, does utf8 calculations
** and on finding an invalid utf8 int (<0 || > 1114111) returns an error value
*/

int	ipf__get_arg_string(t_pfdata **pfdata, t_spec *fmt)
{
	if (fmt->length == LNG_LONG)
	{
		fmt->arg.ls = va_arg((*pfdata)->ap, int *);
		if (fmt->arg.ls == NULL)
			ipf_get_arg_string_null(fmt, 1);
		else if (fmt->precision.str[0] != 0)
			fmt->strlen.value = ipf__utf8_strnlen(fmt->arg.ls,
					fmt->precision.value);
		else
			fmt->strlen.value = ipf__utf8_strlen(fmt->arg.ls);
		if (fmt->strlen.value == (size_t)FT_UTF8_ERR)
			return (FT_UTF8_ERR);
	}
	else
	{
		fmt->arg.s = va_arg((*pfdata)->ap, char *);
		if (fmt->arg.s == NULL)
			ipf_get_arg_string_null(fmt, 0);
		else if (fmt->precision.str[0] != 0)
			fmt->strlen.value = ft_strnlen(fmt->arg.s, fmt->precision.value);
		else
			fmt->strlen.value = ft_strlen(fmt->arg.s);
	}
	return (1);
}
