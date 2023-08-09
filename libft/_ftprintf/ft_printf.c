/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/02/14 11:13:08 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 15:48:52 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

/*
** Function printf
**
** Write to stdout from formatstring ad variadic arguments
** Internally calls vdprintf with fd=1 and va_list
*/

int	ft_printf(const char *fmt, ...)
{
	va_list		ap;
	int			n;

	if (fmt == NULL)
		ft_segdefault();
	va_start(ap, fmt);
	n = ft_vdprintf(1, fmt, ap);
	va_end(ap);
	return (n);
}

/*
** Function dprintf
**
** Write to file descriptor from formatstring and variadic arguments
** Internally calls vdprintf with va_list
*/

int	ft_dprintf(int fd, const char *fmt, ...)
{
	va_list		ap;
	int			n;

	if (fmt == NULL)
		ft_segdefault();
	va_start(ap, fmt);
	n = ft_vdprintf(fd, fmt, ap);
	va_end(ap);
	return (n);
}

/*
** Function asprintf
**
** Sets *ret to pointer to malloced string large enough to hold formatted string
** String is null terminated.
** If malloc fails *ret is set to NULL and function returns -1;
*/

int	ft_asprintf(char **ret, const char *fmt, ...)
{
	va_list		ap;
	int			n;

	if (fmt == NULL)
		ft_segdefault();
	va_start(ap, fmt);
	n = ft_vasprintf(ret, fmt, ap);
	va_end(ap);
	return (n);
}

/*
** Function sprintf
**
** Write to user defined char str
*/

int	ft_sprintf(char *str, const char *fmt, ...)
{
	va_list		ap;
	int			n;

	if (fmt == NULL)
		ft_segdefault();
	va_start(ap, fmt);
	n = ft_vsprintf(str, fmt, ap);
	va_end(ap);
	return (n);
}
