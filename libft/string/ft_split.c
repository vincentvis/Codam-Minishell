/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_split.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/05 11:08:52 by vvissche      #+#    #+#                 */
/*   Updated: 2022/04/12 14:16:16 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static char	*skip_part(char *str, char c, char i)
{
	if (i)
	{
		while (*str == c && *str)
			str++;
	}
	else
	{
		while (!(*str == c) && *str)
			str++;
	}
	return (str);
}

static char	*add_part(char *str, char c)
{
	int		len;
	char	*part;

	len = 0;
	while (!(str[len] == c) && str[len])
		len++;
	part = malloc(sizeof(*part) * (len + 1));
	if (part == NULL)
		return (NULL);
	ft_memcpy(part, str, len);
	part[len] = '\0';
	return (part);
}

static char	**count_parts(char *str, char c)
{
	int		part_count;

	part_count = 0;
	while (*str != '\0')
	{
		str = skip_part(str, c, 1);
		if (*str != '\0')
			part_count++;
		str = skip_part(str, c, 0);
	}
	return (malloc(sizeof(char *) * (part_count + 1)));
}

static void	*ft_fail_split(char ***parts, int i)
{
	char	**p;

	p = *parts;
	while (i >= 0)
	{
		free(p[i]);
		i--;
	}
	free(p);
	return (NULL);
}

char	**ft_split(const char *str, char c)
{
	int		i;
	char	**parts;

	i = 0;
	parts = count_parts((char *)str, c);
	if (parts == NULL)
		return (NULL);
	str = skip_part((char *)str, c, 1);
	while (*str != '\0')
	{
		parts[i] = add_part((char *)str, c);
		if (parts[i] == NULL)
			return (ft_fail_split(&parts, i));
		i++;
		str = skip_part((char *)str, c, 0);
		str = skip_part((char *)str, c, 1);
	}
	parts[i] = 0;
	return (parts);
}
