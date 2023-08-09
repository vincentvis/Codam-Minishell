/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exitcode.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/26 17:45:24 by vincent       #+#    #+#                 */
/*   Updated: 2022/05/26 18:17:20 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

static int	*exit_code(void)
{
	static int	exitcode = 0;

	return (&exitcode);
}

int	store_exit_code(int code)
{
	int *const	exitcode_ptr = exit_code();

	*exitcode_ptr = code;
	return (code);
}

int	get_last_exit_code(void)
{
	return (*exit_code());
}
