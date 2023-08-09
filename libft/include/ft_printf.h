/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/01/27 09:55:15 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 15:48:18 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <stdlib.h>

/*
** Functions callable from the "oudside"
** The rest is in the "ft_printf_int.h" header
*/

int		ft_printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
int		ft_dprintf(int fd,
			const char *fmt, ...) __attribute__((format(printf, 2, 3)));
int		ft_vdprintf(int fd, const char *fmt,
			va_list ap) __attribute__((format(printf, 2, 0)));
int		ft_asprintf(char **ret,
			const char *fmt, ...) __attribute__((format(printf, 2, 3)));
int		ft_vasprintf(char **ret, const char *fmt,
			va_list ap) __attribute__((format(printf, 2, 0)));
int		ft_sprintf(char *ret,
			const char *fmt, ...) __attribute__((format(printf, 2, 3)));
int		ft_vsprintf(char *ret, const char *fmt,
			va_list ap) __attribute__((format(printf, 2, 0)));

#endif
