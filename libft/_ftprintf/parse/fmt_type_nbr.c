/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fmt_type_nbr.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/17 12:56:54 by vincent       #+#    #+#                 */
/*   Updated: 2022/04/12 15:49:30 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

/*
** Function to calculate positions of string parts
**
** Description
** Takes the already calculated lengths to determine where parts of the string
** should start.
**
** Parameters
** [t_spec]			{fmt}		spec of current fmt
*/

static void	ipf__fmt_type_nbr_calc_position(t_spec *fmt)
{
	fmt->precision.start = fmt->flags.length;
	fmt->strlen.start = fmt->flags.length + fmt->precision.length;
	if (fmt->flags.value & FLG_MINU)
	{
		fmt->width.str[0] = ' ';
		fmt->flags.start = 0;
		fmt->width.start = fmt->flags.length + fmt->precision.length
			+ fmt->strlen.value;
	}
	else
	{
		fmt->precision.start += fmt->width.length;
		fmt->strlen.start += fmt->width.length;
		fmt->flags.start = fmt->width.length;
		if (fmt->precision.str[0] != 0)
			fmt->width.str[0] = ' ';
		if ((fmt->flags.value & FLG_ZERO) && fmt->width.str[0] == '0')
		{
			fmt->flags.start = 0;
			fmt->width.start += fmt->flags.length;
		}
	}
}

/*
** Function to calculate sizes of string parts
**
** Description
** Takes the input from the format string to determine the actual length
** of the parts of the string. Also adds the total amount of chars that will
** be printed to the total in the pfdata struct
**
** Parameters
** [t_pfdata]		{pfdata}	Pointer of main data object
** [t_spec]			{fmt}		spec of current fmt
*/

static void	ipf__fmt_type_nbr_calc_size(t_pfdata **pfdata, t_spec *fmt)
{
	fmt->strlen.length = fmt->strlen.value + fmt->flags.length;
	if (fmt->precision.value >= fmt->strlen.value && fmt->precision.str[0] != 0)
		fmt->strlen.length = fmt->precision.value + fmt->flags.length;
	if (fmt->width.value > fmt->strlen.length)
		fmt->strlen.length = fmt->width.value;
	(*pfdata)->t_len += fmt->strlen.length;
	if (fmt->precision.value > fmt->strlen.value)
		fmt->precision.length = fmt->precision.value - fmt->strlen.value;
	if (fmt->width.value
		> (fmt->precision.length + fmt->strlen.value + fmt->flags.length))
		fmt->width.length = fmt->width.value
			- (fmt->precision.length + fmt->strlen.value + fmt->flags.length);
	ipf__fmt_type_nbr_calc_position(fmt);
}

/*
** Function to set everything for conversion type nbr
**
** Parameters
** [t_pfdata]		{pfdata}	Pointer of main data object
** [t_spec]			{fmt}		spec of current fmt
** [uint_fast8_t]	{action}	0 for print 1 for to str
*/

int	ipf__fmt_type_nbr(t_pfdata **pfdata, t_spec *fmt)
{
	ipf__get_arg_signed(pfdata, fmt);
	if (fmt->arg.lli < 0)
	{
		fmt->arg.lli *= -1;
		fmt->flags.length = 1;
		fmt->flags.str[0] = '-';
	}
	else if ((fmt->flags.value & (FLG_PLUS | FLG_SPAC)))
	{
		fmt->flags.str[0] = ' ' + (((fmt->flags.value & FLG_PLUS) >> 4) * 11);
		fmt->flags.length = 1;
	}
	fmt->strlen.value = ft_nbrlen_unsigned(fmt->arg.lli);
	if (fmt->arg.lli == 0 && fmt->precision.str[0] != 0)
		fmt->strlen.value = 0;
	ipf__fmt_type_nbr_calc_size(pfdata, fmt);
	return (1);
}

/*
** Function to set everything for conversion type pointer
**
** Parameters
** [t_pfdata]		{pfdata}	Pointer of main data object
** [t_spec]			{fmt}		Spec of current fmt
*/

int	ipf__fmt_type_pointer(t_pfdata **pfdata, t_spec *fmt)
{
	fmt->arg.llu = (unsigned long long)va_arg((*pfdata)->ap, void *);
	fmt->type = 'x';
	fmt->flags.value |= FLG_HASH;
	fmt->flags.length = 2;
	fmt->flags.value &= ~(FLG_PLUS | FLG_SPAC);
	if (fmt->arg.llu == 0 && fmt->precision.str[0] != 0
		&& fmt->precision.value == 0)
		fmt->strlen.value = 0;
	else
		fmt->strlen.value = ft_nbrlen_unsigned_base16(fmt->arg.llu);
	ipf__fmt_type_nbr_calc_size(pfdata, fmt);
	return (1);
}

/*
** Function to set everything for conversion type nbr_base
**
** Parameters
** [t_pfdata]		{pfdata}	Pointer of main data object
** [t_spec]			{fmt}		spec of current fmt
*/

int	ipf__fmt_type_nbr_base(t_pfdata **pfdata, t_spec *fmt)
{
	ipf__get_arg_unsigned(pfdata, fmt);
	if (fmt->type == 'x' || fmt->type == 'X')
	{
		fmt->strlen.value = ft_nbrlen_unsigned_base16(fmt->arg.llu);
		fmt->flags.length += (fmt->flags.value & FLG_HASH) >> 1;
		fmt->flags.str[1] = 'X' + ((fmt->type == 'x') * 32);
	}
	if (fmt->type == 'o')
	{
		fmt->strlen.value = ft_nbrlen_unsigned_base8(fmt->arg.llu);
		if (fmt->precision.value <= fmt->strlen.value)
			fmt->flags.length += (fmt->flags.value & FLG_HASH) >> 2;
	}
	if (fmt->type == 'u')
		fmt->strlen.value = ft_nbrlen_unsigned(fmt->arg.llu);
	if (fmt->arg.llu == 0)
		fmt->flags.length = 0;
	if (fmt->precision.str[0] != 0 && fmt->arg.llu == 0)
		fmt->strlen.value = 0;
	if (fmt->type == 'o' && fmt->arg.llu == 0 && fmt->precision.str[0] != 0
		&& fmt->precision.value == 0 && fmt->flags.value & FLG_HASH)
		fmt->flags.length = 1;
	ipf__fmt_type_nbr_calc_size(pfdata, fmt);
	return (1);
}
