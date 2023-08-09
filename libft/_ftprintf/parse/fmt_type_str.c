/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fmt_type_str.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/17 12:56:54 by vincent       #+#    #+#                 */
/*   Updated: 2022/04/12 15:49:33 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

/*
** Fmt calculation for type string
**
** Description
** Calculation of sizes of all individual parts for all string related things
**
** Parameters
** [t_pfdata]		{pfdata}	Pointer of main data object
** [t_spec]			{fmt}		Current t_spec
*/

static void	ipf__fmt_type_str_calc(t_pfdata **pfdata, t_spec *fmt)
{
	fmt->strlen.length = fmt->strlen.value;
	if (fmt->width.value > fmt->strlen.value)
	{
		fmt->strlen.length = fmt->width.value;
		fmt->width.length = fmt->width.value - fmt->strlen.value;
		fmt->strlen.start = fmt->width.length;
	}
	if (fmt->flags.value & FLG_MINU)
	{
		fmt->width.start = fmt->strlen.value;
		fmt->strlen.start = 0;
	}
	if ((fmt->flags.value & FLG_ZERO) && (fmt->flags.value & FLG_MINU))
		fmt->width.str[0] = ' ';
	(*pfdata)->t_len += fmt->strlen.length;
}

/*
** Function to set everything for conversion type char
** If get_arg fails (on some urf8 cases) it returns -1 down the chain
**
** Parameters
** [t_pfdata]		{pfdata}	Pointer of main data object
** [t_spec]			{fmt}		Current t_spec
*/

int	ipf__fmt_type_char(t_pfdata **pfdata, t_spec *fmt)
{
	fmt->strlen.value = 1;
	if (fmt->type == '%')
		fmt->arg.lli = '%';
	else
	{
		if (fmt->length == LNG_LONG)
		{
			fmt->arg.lli = va_arg((*pfdata)->ap, long long);
			if (ipf__utf8_width(fmt->arg.lli) == FT_UTF8_ERR)
				return (FT_UTF8_ERR);
			fmt->strlen.value = ipf__utf8_width(fmt->arg.lli);
		}
		else
			fmt->arg.lli = (unsigned char)va_arg((*pfdata)->ap, int);
	}
	ipf__fmt_type_str_calc(pfdata, fmt);
	return (1);
}

/*
** Function to set everything for conversion type string
** If get_arg fails (on some utf8 cases) it resturns -1 down the chain
**
** Parameters
** [t_pfdata]		{pfdata}	Pointer of main data object
** [t_spec]			{fmt}		Current t_spec
*/

int	ipf__fmt_type_string(t_pfdata **pfdata, t_spec *fmt)
{
	if (ipf__get_arg_string(pfdata, fmt) == FT_UTF8_ERR)
		return (FT_UTF8_ERR);
	ipf__fmt_type_str_calc(pfdata, fmt);
	return (1);
}

/*
** Invalid Type exception
**
** When there is an invalid type after start of format printf just prints
** the char that is there. i do the same :)
**
** Parameters
** [t_pfdata]		{pfdata}	Pointer of main data object
** [size_t]			{i}			Index of where we are in fmt
*/

int	ipf__fmt_type_invalid(t_pfdata **pfdata, size_t i)
{
	if ((*pfdata)->fmt[i] == 0)
	{
		(*pfdata)->tail->width.value = 0;
		(*pfdata)->tail->strlen.value = 0;
		(*pfdata)->tail->flags.value = 0;
		(*pfdata)->tail->precision.value = 0;
		(*pfdata)->tail->length = 0;
		(*pfdata)->tail->type = 'c';
		return (0);
	}
	else
	{
		(*pfdata)->tail->type = 'c';
		(*pfdata)->tail->arg.lli = (*pfdata)->fmt[i];
		(*pfdata)->tail->strlen.value = 1;
		ipf__fmt_type_str_calc(pfdata, (*pfdata)->tail);
		return (1);
	}
}
