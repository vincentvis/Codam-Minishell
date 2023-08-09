/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_nbr.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/27 13:51:22 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 16:00:06 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NBR_H
# define FT_NBR_H

# include <stdint.h>

typedef struct s_basemap
{
	uint_fast8_t	base;
	uint_fast8_t	value;
}					t_basemap;

int				ft_nbrtoastr(char **ret, unsigned long long nbr,
					int_fast8_t sign);
int				ft_nbrtoastr_n(char **ret, unsigned long long nbr,
					uint_fast8_t nbrlen, int_fast8_t sign);
int				ft_nbrtostr(char *dst, unsigned long long nbr,
					int_fast8_t sign);
int				ft_nbrtostr_n(char *dst, unsigned long long nbr,
					uint_fast8_t nbrlen, int_fast8_t sign);

int				ft_nbrtoastr_base16(char **ret, unsigned long long nbr,
					int_fast8_t uplow);
int				ft_nbrtoastr_n_base16(char **ret, unsigned long long nbr,
					uint_fast8_t nbrlen, int_fast8_t uplow);
int				ft_nbrtostr_base16(char *dst, unsigned long long nbr,
					int_fast8_t uplow);
int				ft_nbrtostr_n_base16(char *dst, unsigned long long nbr,
					uint_fast8_t nbrlen, int_fast8_t uplow);

int				ft_nbrtoastr_base2(char **ret, unsigned long long nbr);
int				ft_nbrtostr_base2(char *dst, unsigned long long nbr);
int				ft_nbrtostr_n_base2(char *dst, unsigned long long nbr,
					uint_fast8_t nbrlen);

int				ft_nbrtostr_base(char *dst, unsigned long long nbr,
					uint_fast8_t base);
int				ft_nbrtoastr_base(char **dst, unsigned long long nbr,
					uint_fast8_t base);

const short		*get_base10_short(void) __attribute__((always_inline));
const char		*get_base16(void) __attribute__((always_inline));
const char		*get_base36(void) __attribute__((always_inline));

#endif
