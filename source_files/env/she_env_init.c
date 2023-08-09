/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   she_env_init.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/08 16:38:41 by vincent       #+#    #+#                 */
/*   Updated: 2022/06/07 17:58:53 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "env/she_env.h"
#include "env/she_env_internal.h"

static void	free_env_var_list(t_env_var *start)
{
	t_env_var	*var;
	t_env_var	*temp;

	var = start;
	while (var)
	{
		if (var->data)
			free(var->data);
		var->data = NULL;
		temp = var->next;
		free(var);
		var = temp;
	}
}

void	sheenv_destroy(void)
{
	t_env_base *const	env_base = sheenv();

	if (env_base->initialised && env_base->head)
		free_env_var_list(env_base->head);
	env_base->head = NULL;
	env_base->tail = NULL;
	env_base->count_keys = 0;
	env_base->count_values = 0;
	env_base->initialised = false;
}

static t_response	handle_failed_init(
		t_env_base *env_base,
		t_response response
	)
{
	free_env_var_list(env_base->head);
	env_base->head = NULL;
	env_base->tail = NULL;
	env_base->count_keys = 0;
	env_base->count_values = 0;
	env_base->initialised = false;
	return (response);
}

static t_response	set_default_values_in_env(t_env_base *const env_base)
{
	t_response	oldpwd;
	t_response	shlvl;
	int			shlvl_value;
	char		lvl_str[5];

	sheenv_unset("OLDPWD");
	oldpwd = sheenv_store_from_input("OLDPWD");
	if (oldpwd.status != SUCCESS)
		return (oldpwd);
	shlvl_value = 0;
	shlvl = sheenv_get_value_by_key("SHLVL");
	if (shlvl.status == SUCCESS && shlvl.data != NULL)
		shlvl_value = ft_atoi(shlvl.data);
	if (shlvl_value >= 1000)
		shlvl_value = 0;
	ft_nbrtostr(lvl_str, shlvl_value + 1, 0);
	if (shlvl_value == 999)
		ft_memcpy(lvl_str, "", 1);
	shlvl = sheenv_store_from_key_value("SHLVL", lvl_str);
	if (shlvl.status != SUCCESS)
		return (shlvl);
	return (make_response(SUCCESS, env_base));
}

t_response	sheenv_init(char const **env)
{
	t_env_base *const	env_base = sheenv();
	t_response			new_var;
	size_t				i;

	if (env_base->initialised == true)
		return (make_response(SHEENV_DOUBLE_INIT, NULL));
	if (env == NULL)
		return (make_response(SHEENV_BAD_INPUT, NULL));
	env_base->initialised = true;
	i = 0;
	while (env[i])
	{
		new_var = sheenv_store_from_input(env[i]);
		if (new_var.status != SUCCESS)
			return (handle_failed_init(env_base, new_var));
		i++;
	}
	return (set_default_values_in_env(env_base));
}
