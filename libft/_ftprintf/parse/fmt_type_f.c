/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fmt_type_f.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/07 18:37:33 by vincent       #+#    #+#                 */
/*   Updated: 2022/04/12 15:49:20 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

static void	ipf__fmt_type_double_calc_position(t_spec *fmt)
{
	if (fmt->width.value > (fmt->strlen.value + fmt->flags.length))
		fmt->width.length = fmt->width.value
			- (fmt->strlen.value + fmt->flags.length);
	fmt->strlen.start = fmt->flags.length;
	if (fmt->flags.value & FLG_MINU)
	{
		fmt->width.str[0] = ' ';
		fmt->flags.start = 0;
		fmt->width.start = fmt->flags.length + fmt->strlen.value;
	}
	else
	{
		fmt->strlen.start += fmt->width.length;
		fmt->flags.start = fmt->width.length;
		if ((fmt->flags.value & FLG_ZERO) && fmt->width.str[0] == '0')
		{
			fmt->flags.start = 0;
			fmt->width.start += fmt->flags.length;
		}
	}
	if (fmt->precision.value >= BIGINT_DECIMAL_MAX - 1)
		fmt->precision.start = fmt->width.length + fmt->flags.start
			+ fmt->strlen.value;
}

static void	ipf__fmt_type_double_edgecases(t_spec *fmt)
{
	if (fmt->bigint->len_decimal == 1 && fmt->precision.value == 0
		&& fmt->bigint->str[BIGINT_DECIMAL_POS + 1] == '0'
		&& !(fmt->flags.value & FLG_HASH))
		fmt->bigint->endpos--;
	if ((fmt->bigint->is_nan || fmt->bigint->is_inf) && fmt->type == 'F')
	{
		fmt->bigint->startpos += 5;
		fmt->bigint->endpos += 5;
	}
	if ((fmt->flags.value & (FLG_PLUS | FLG_SPAC))
		&& fmt->bigint->str[fmt->bigint->startpos] != '-')
	{
		fmt->flags.str[0] = ' ' + (((fmt->flags.value & FLG_PLUS) >> 4) * 11);
		if (fmt->bigint->str[fmt->bigint->startpos] == '-')
			fmt->flags.str[0] = '-';
		fmt->flags.length = 1;
	}
	if (fmt->bigint->is_nan || fmt->bigint->is_inf)
		fmt->width.str[0] = ' ';
	if (fmt->bigint->is_nan)
		fmt->flags.length = 0;
}

int	ipf__fmt_type_double(t_pfdata **pfdata, t_spec *fmt)
{
	int	round_to;

	ipf__get_arg_double(pfdata, fmt);
	fmt->bigint = ft_bigint_from_double(NULL, fmt->arg.f);
	if (fmt->precision.str[0] != '0')
		fmt->precision.value = 6;
	round_to = fmt->precision.value;
	if (fmt->precision.value >= BIGINT_DECIMAL_MAX - 1)
		round_to = BIGINT_DECIMAL_MAX - 2;
	ft_bigint_round(fmt->bigint, round_to);
	ipf__fmt_type_double_edgecases(fmt);
	fmt->strlen.value = fmt->bigint->endpos - fmt->bigint->startpos;
	fmt->strlen.length = fmt->strlen.value + fmt->flags.length;
	if (fmt->precision.value >= BIGINT_DECIMAL_MAX - 1)
		fmt->strlen.length += (fmt->precision.value - BIGINT_DECIMAL_MAX + 2);
	if (fmt->width.value > fmt->strlen.length)
		fmt->strlen.length = fmt->width.value;
	(*pfdata)->t_len += fmt->strlen.length;
	ipf__fmt_type_double_calc_position(fmt);
	return (1);
}
