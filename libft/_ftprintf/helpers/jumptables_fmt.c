/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   jumptables_fmt.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/04/12 15:52:40 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 15:58:56 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_int.h"

/*
** Static const map for character stuff
**
** Array filled with 0's an on positions where i need it a value
** Positions of flag characters (' '=32,'#'=35,'+'=43,'-'=45,'0'=48)
** mapped to 2^n ('0'=n1,'-'=n2,'#'=n3,' '=n4,'+'=n5) - max = 16
** And allowed fmt chars with just their ascii value (anything above 16)
** This way we can use the same array for flags and types
*/

const uint_fast8_t	*ipf_char_map(void)
{
	static const uint_fast8_t	ipf_map[256] = {
	['0'] = 1U,
	['-'] = 2U,
	['#'] = 4U,
	[' '] = 8U,
	['+'] = 16U,
	['c'] = 'c',
	['d'] = 'd',
	['i'] = 'i',
	['n'] = 'n',
	['o'] = 'o',
	['p'] = 'p',
	['s'] = 's',
	['u'] = 'u',
	['x'] = 'x',
	['X'] = 'X',
	['f'] = 'f',
	['F'] = 'F',
	};

	return (ipf_map);
}

/*
** Jump table for format processing
**
** Has enough locations to store all conversion characters minus some predifined
** number to restrict the size of the array and conserve (some..) memory
** Size is determined by last (ascii order) possible type - JMP_OFFST
**
** Each element has a function pointer to the function that handles that
** conversion.
*/

const t_jmp	*ipf_jump_format(void)
{
	static const t_jmp	jumptable['x' - JMP_OFFST + 1] = {
	['%' - JMP_OFFST] = &ipf__fmt_type_char,
	['c' - JMP_OFFST] = &ipf__fmt_type_char,
	['s' - JMP_OFFST] = &ipf__fmt_type_string,
	['t' - JMP_OFFST] = &ipf__fmt_type_string,
	['p' - JMP_OFFST] = &ipf__fmt_type_pointer,
	['n' - JMP_OFFST] = &ipf__fmt_type_n,
	['i' - JMP_OFFST] = &ipf__fmt_type_nbr,
	['d' - JMP_OFFST] = &ipf__fmt_type_nbr,
	['u' - JMP_OFFST] = &ipf__fmt_type_nbr_base,
	['o' - JMP_OFFST] = &ipf__fmt_type_nbr_base,
	['x' - JMP_OFFST] = &ipf__fmt_type_nbr_base,
	['X' - JMP_OFFST] = &ipf__fmt_type_nbr_base,
	['f' - JMP_OFFST] = &ipf__fmt_type_double,
	['F' - JMP_OFFST] = &ipf__fmt_type_double,
	};

	return (jumptable);
}

/*
** Jump table for format processing
**
** Has enough locations to store all conversion characters minus some predifined
** number to restrict the size of the array and conserve memory
**
** Each element has a function pointer to the function that handles that
** conversion.
*/

const t_jmp	*ipf_jump_process(void)
{
	static const t_jmp	jumptable['x' - JMP_OFFST + 1] = {
	['%' - JMP_OFFST] = &ipf__process_char,
	['c' - JMP_OFFST] = &ipf__process_char,
	['s' - JMP_OFFST] = &ipf__process_string,
	['t' - JMP_OFFST] = &ipf__process_string,
	['p' - JMP_OFFST] = &ipf__process_nbr_base,
	['n' - JMP_OFFST] = &ipf__process_n,
	['i' - JMP_OFFST] = &ipf__process_nbr,
	['d' - JMP_OFFST] = &ipf__process_nbr,
	['u' - JMP_OFFST] = &ipf__process_nbr,
	['o' - JMP_OFFST] = &ipf__process_nbr_base,
	['x' - JMP_OFFST] = &ipf__process_nbr_base,
	['X' - JMP_OFFST] = &ipf__process_nbr_base,
	['f' - JMP_OFFST] = &ipf__process_double,
	['F' - JMP_OFFST] = &ipf__process_double,
	};

	return (jumptable);
}
