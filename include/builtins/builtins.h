/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: vvissche <vvissche@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/05/11 13:51:09 by vvissche      #+#    #+#                 */
/*   Updated: 2022/05/26 16:25:02 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int	builtin_echo(char *const *args);
int	builtin_cd(char *const *args);
int	builtin_pwd(char *const *args);
int	builtin_export(char *const *args);
int	builtin_unset(char *const *args);
int	builtin_env(char *const *args);
int	builtin_exit(char *const *args);
#endif
