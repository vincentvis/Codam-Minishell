/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_vprintf.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/03/25 12:34:24 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/10 17:36:35 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "ft_printf_int.h"

/*
** Function Create PFdata (static)
**
** Description
** Allocates t_pfdata struct and sets default values
**
** Parameters
** [t_pfdata **]	{pfdata}	pointer to where to put the struct
** [const char *]	fmt			Pointer to begin of formatstring
** [va_list]		ap			list from stdarg
*/

static int	ipf__create_pfdata(t_pfdata **pfdata, const char *fmt, va_list ap)
{
	*pfdata = ft_calloc(1, sizeof(**pfdata));
	if (!(*pfdata))
		return (-1);
	(*pfdata)->fmt = fmt;
	(*pfdata)->dest = NULL;
	(*pfdata)->head = NULL;
	(*pfdata)->tail = NULL;
	va_copy((*pfdata)->ap, ap);
	return (1);
}

/*
** Function vdprintf
**
** Write to file descriptor from format string and va_list
*/

int	ft_vdprintf(int fd, const char *fmt, va_list ap)
{
	t_pfdata	*pfdata;
	int			ret;
	int			t_len;

	if (ipf__create_pfdata(&pfdata, fmt, ap) == -1)
		return (-1);
	pfdata->fd = fd;
	ipf__build_list(&pfdata);
	ipf__process_list(&pfdata, &pfdata->dest, 1);
	if (pfdata->error < 0)
	{
		free(pfdata->dest);
		return (ipf__destroy_list(&pfdata));
	}
	ret = write(pfdata->fd, pfdata->dest, pfdata->t_len);
	free(pfdata->dest);
	t_len = pfdata->t_len;
	if (pfdata->error == 2 || ret == -1)
		t_len = -1;
	free(pfdata);
	return (t_len);
}

/*
** Function vasprintf
**
** Write to malloced destination pointer from format string and va_list
*/

int	ft_vasprintf(char **ret, const char *fmt, va_list ap)
{
	t_pfdata	*pfdata;
	int			t_len;

	if (ipf__create_pfdata(&pfdata, fmt, ap) == -1)
		return (-1);
	ipf__build_list(&pfdata);
	ipf__process_list(&pfdata, ret, 1);
	if (pfdata->error < 0)
		return (ipf__destroy_list(&pfdata));
	t_len = pfdata->t_len;
	if (pfdata->error == 2)
		t_len = -1;
	free(pfdata);
	return (t_len);
}

/*
** Funcition vsprintf
**
** Write to char pointer assuming enough length for format
**
** !! dangerous, never use with user input !!
*/

int	ft_vsprintf(char *str, const char *fmt, va_list ap)
{
	t_pfdata	*pfdata;
	int			t_len;

	if (ipf__create_pfdata(&pfdata, fmt, ap) == -1)
		return (-1);
	ipf__build_list(&pfdata);
	ipf__process_list(&pfdata, &str, 0);
	if (pfdata->error < 0)
		return (ipf__destroy_list(&pfdata));
	t_len = pfdata->t_len;
	if (pfdata->error == 2)
		t_len = -1;
	free(pfdata);
	return (t_len);
}
