/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf_int.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/20 10:51:08 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 16:00:19 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_INT_H
# define FT_PRINTF_INT_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"
# include <stdint.h>

/*# ifndef CAN_INCLUDE_FT_PRINTF_INTERNAL
#  error "Never include "ft_printf_int.h" directly; use "ft_printf.h" instead."
# endif*/

# define STR_NULL "(null)"
# define STR_NULL_L L"(null)"
# define JMP_OFFST 37
# define FT_UTF8_MAX 0x10FFFF
# define FT_UTF8_ERR -2

/*
** define for containing flags
**
** Flags can be set with FLG_ZERO variable names, or numbers 2^n
** Comparison can be done like this for single flags:
** if (flag & FLG_ZERO)
** and for testing multiple flags:
** if (flag & (FLG_ZERO | FLG_HASH))
*/

# define FLG_ZERO 1U
# define FLG_MINU 2U
# define FLG_HASH 4U
# define FLG_SPAC 8U
# define FLG_PLUS 16U

/*
** Enum for length modifier (BONUS)
**
** Using enum so the LNG_CHAR names are available for easy checking
*/

typedef enum e_length
{
	LNG_CHAR = 1 << 1,
	LNG_SHRT = 1 << 2,
	LNG_LONG = 1 << 3,
	LNG_LLNG = 1 << 4,
	LNG_DOUB = 1 << 5,
	LNG_SIZT = 1 << 6,
	LNG_IMAX = 1 << 7,
	LNG_PTRT = 1 << 8
}						t_length;

/*
** Union for arguments
**
** Use of union so we can use single variable to save all possible types
*/

typedef union u_arg
{
	long long			lli;
	unsigned long long	llu;
	size_t				zu;
	char				*s;
	int					*ls;
	void				*p;
	double				f;
}						t_arg;

/*
** Struct for saving lengths of precision/width/str/nbr
**
** Use for example like this:
** memset(&dest[precision.start], str[0], precision.length);
**
** [size_t]				{value}		orig. value (without calculation performed)
** [size_t]				{start}		start pos in str
** [size_t]				{length}	length of part
** [char *]				{str}		to be flexible with what char to write
*/

typedef struct s_part
{
	size_t				value;
	size_t				start;
	size_t				length;
	char				str[2];
}						t_part;

/*
** Struct with format specifications
**
** Each time an '%' is found, there should be one of these
**
** [t_part]				{flags}		struct for storing flag information
** [t_part]				{width}		struct for storing width information
** [t_part]				{precision}	struct for storing precision information
** [t_length]			{length}	enum for storing lengths
** [char]				{type}		char for storing type
** [t_arg]				{arg}		union for storing values (va_arg etc)
** [t_part]				{strlen}	struct for string string length information
** [struct s_spec *]	{next}		pointer to the next t_spec
*/

typedef struct s_spec
{
	t_part				flags;
	t_part				width;
	t_part				precision;
	t_length			length;
	char				type;
	t_arg				arg;
	t_part				strlen;
	t_bigint			*bigint;
	struct s_spec		*next;
}						t_spec;

/*
** Struct with main printf data
**
** This struct is generated when calling printf and is sort of the access point
** for everything printf related
**
** [int]			{fd}		File descriptor to write to
** [int]			{error}		Integer to store error values in
** [const char *]	{fmt}		Char array containing format string
** [char]			{dest}		Destination to write to
** [size_t]			{pos}		Current position of writing in my destination
** [size_t]			{t_len}		Total len of str that will be created for printf
** [va_list]		{ap}		Variable argument lis
** [t_spec *]		{head}		pointer to head of linked list
** [t_spec *]		{tail}		pointer to tail of linked list
*/

typedef struct s_pfdata
{
	int					fd;
	int					error;
	const char			*fmt;
	char				*dest;
	size_t				pos;
	size_t				t_len;
	va_list				ap;
	t_spec				*head;
	t_spec				*tail;
}						t_pfdata;

/*
** char				{sign}		0 = lowercase -32 = uppercase
*/

/*
** Function Pointer for jump table
*/

typedef int	(*t_jmp)(t_pfdata **, t_spec *);

/*
** Functions that should not be called from outside printf library
** but can't be static because of 'max functions per file' norm restriction
** To indicate that they are local functions they start with ipf__
*/

/*
** Base
*/

int						ipf__build_list(t_pfdata **pfdata);
size_t					ipf__process_list(t_pfdata **pfdata, char **dst, int m);
int						ipf__destroy_list(t_pfdata **pfdata);
int						ipf__handle_parse_error(t_pfdata **pfdata);

/*
** Process List internals
*/

int						ipf__process_char(t_pfdata **pfdata, t_spec *fmt);
int						ipf__process_string(t_pfdata **pfdata, t_spec *fmt);
int						ipf__process_nbr(t_pfdata **pfdata, t_spec *fmt);
int						ipf__process_nbr_base(t_pfdata **pfdata, t_spec *fmt);
int						ipf__process_n(t_pfdata **pfdata, t_spec *fmt);
int						ipf__process_double(t_pfdata **pfdata, t_spec *fmt);

/*
** Parse FMT internals
*/

size_t					ipf__parse_flags(t_pfdata **pfdata, size_t i);
size_t					ipf__parse_width(t_pfdata **pfdata, size_t i);
size_t					ipf__parse_precision(t_pfdata **pfdata, size_t i);
void					ipf__parse_precision_undef(t_pfdata **pfdata, int prec);
size_t					ipf__parse_length(t_pfdata **pfdata, size_t i);
size_t					ipf__parse_type(t_pfdata **pfdata, size_t i);

int						ipf__fmt_type_char(t_pfdata **pfdata, t_spec *fmt);
int						ipf__fmt_type_string(t_pfdata **pfdata, t_spec *fmt);
int						ipf__fmt_type_invalid(t_pfdata **pfdata, size_t i);
int						ipf__fmt_type_pointer(t_pfdata **pfdata, t_spec *fmt);
int						ipf__fmt_type_nbr(t_pfdata **pfdata, t_spec *fmt);
int						ipf__fmt_type_nbr_base(t_pfdata **pfdata, t_spec *fmt);
int						ipf__fmt_type_n(t_pfdata **pfdata, t_spec *fmt);

// work in progress
int						ipf__fmt_type_double(t_pfdata **pfdata, t_spec *fmt);
void					ipf__get_arg_double(t_pfdata **pfdata, t_spec *fmt);

void					ipf__get_arg_signed(t_pfdata **pfdata, t_spec *fmt);
void					ipf__get_arg_unsigned(t_pfdata **pfdata, t_spec *fmt);
int						ipf__get_arg_string(t_pfdata **pfdata, t_spec *fmt);

/*
** UTF8 stuff
*/

int						ipf__utf8_width(int wchar);
int						ipf__utf8_strlen(int *wstr);
int						ipf__utf8_strnlen(int *wstr, int max);
void					ipf__utf8(char *dest, int input, int width);
int						ipf__utf8_str(char *dest, int *input, int maxwidth);

/*
Jump tables
*/
const uint_fast8_t		*ipf_char_map(void) __attribute__((always_inline));
const t_jmp				*ipf_jump_format(void) __attribute__((always_inline));
const t_jmp				*ipf_jump_process(void) __attribute__((always_inline));

#endif
