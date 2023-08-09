/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   validation_symbols.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/08 16:38:41 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/26 18:11:24 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

int	ispipe(int c)
{
	return (c == '|');
}

int	isleftchevron(int c)
{
	return (c == '<');
}

int	isrightchevron(int c)
{
	return (c == '>');
}

int	isquestionmark(int c)
{
	return (c == '?');
}
