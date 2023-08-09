/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_bigint.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/10 15:59:59 by vincent       #+#    #+#                 */
/*   Updated: 2021/07/15 14:21:20 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_BIGINT_H
# define FT_BIGINT_H

/*
** DBL_MAX has 309 chars on the left of decimal point
** largest amount of chars on right of decimal point you can get is 1074
** add 1 for decimal point, 1 for sign = 1385.
** Set to 1408 to make it divisible by 64
*/

# define BIGINT_DIGIT_MAX 1000
# define BIGINT_DECIMAL_MAX 1100
# define BIGINT_STR_MAX 2101
# define BIGINT_DECIMAL_POS 1000

typedef struct s_bigint
{
	char	str[BIGINT_STR_MAX + 1];
	char	*num_str;
	int		sign;
	int		len_digit;
	int		len_decimal;
	int		len_dbl;
	int		startpos;
	int		endpos;
	int		is_inf;
	int		is_nan;
}				t_bigint;

typedef struct s_bigint_subtraction
{
	t_bigint	*left;
	t_bigint	*right;
	int			leftmost_pos;
	int			rightmost_pos;
	int			carry;
	int			result;
	int			max;
	int			sign;
}				t_bigint_subtraction;

typedef struct s_dbl
{
	int					sign;
	char				fract_bin[65];
	size_t				frac_len;
	long				exponent_value;
	unsigned long		fraction_value;
	unsigned long		bits;
	unsigned long		left;
	unsigned long		right;
}						t_dbl;

# define DBL_SIGN_MASK 0x8000000000000000
# define DBL_EXP_MASK 0x7FF0000000000000
# define DBL_FRAC_MASK 0xFFFFFFFFFFFFF
# define DBL_BIAS 1023
# define DBL_INF 1024

t_bigint		*ft_bigint_create(void);
void			ft_bigint_set_default(t_bigint *bigint);
void			*ft_bigint_destroy(t_bigint *bigint);
t_bigint		*ft_bigint_round(t_bigint *bigint, int round_to);

t_bigint		*ft_bigint_from_unsigned(t_bigint *bigint,
					unsigned long long num);
t_bigint		*ft_bigint_from_signed(t_bigint *bigint, long long num);
t_bigint		*ft_bigint_from_str(t_bigint *bigint, char *str);

t_bigint		*ft_bigint_from_double(t_bigint *bigint, double dbl);
void			ft_bigint_set_dbl_struct(t_dbl *info, double dbl);
t_bigint		*ft_bigint_double_zero(t_bigint *bigint);
t_bigint		*ft_bigint_inf_or_nan(t_bigint *bigint, t_dbl *info);
t_bigint		*ft_bigint_double_with_str(t_bigint *bigint, t_dbl *info);

/*
** Addition:
*/

void			ft_bigint_add(t_bigint *left, t_bigint *right, t_bigint *dest);
void			ft_bigint_add_str_to_bigint(t_bigint *left, char *str,
					t_bigint *dest);
t_bigint		*ft_bigint_add_str_to_str(char *left_str, char *right_str);
// todo:
void			ft_bigint_add_long_to_bigint(t_bigint *bigint, long add,
					t_bigint *dest);

/*
** Division:
*/

void			ft_bigint_div(t_bigint *bigint, unsigned long long divisor);
void			ft_bigint_div_10(t_bigint *bigint, int n);
t_bigint		*ft_bigint_r_shift(t_bigint *bigint, int n);

/*
** Multiplication:
*/

void			ft_bigint_mul(t_bigint *bigint, unsigned long long multiplier);
void			ft_bigint_mul_10(t_bigint *bigint, int n);
t_bigint		*ft_bigint_l_shift_long(unsigned long long num, int n);
t_bigint		*ft_bigint_l_shift(t_bigint *bigint, int n);

/*
** !!!!!!!TODO!!!!!!!
** Subtraction:
*/
/*
void			ft_bigint_sub(t_bigint *left, t_bigint *right, t_bigint *dest);
void			ft_bigint_sub_int(t_bigint *bigint, int addition);
*/

#endif
