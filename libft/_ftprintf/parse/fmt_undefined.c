/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fmt_undefined.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 13:43:10 by vincent       #+#    #+#                 */
/*   Updated: 2022/04/12 15:49:36 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

/*
** Parse Precision Undefined
**
** This function can be called by fmt.c:ipf__parse_precision()
**
** If the precision comes out of ft_atoi as < 0
** the printf behaviour is different than described in the man
** But since it's tested against printf i've implemented it in the same way
*/

void	ipf__parse_precision_undef(t_pfdata **pfdata, int precision)
{
	(*pfdata)->tail->precision.value = 0;
	(*pfdata)->tail->width.value = precision * -1;
	(*pfdata)->tail->flags.value &= ~FLG_ZERO;
	(*pfdata)->tail->width.str[0] = ' ';
}
