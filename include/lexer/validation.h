/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   validation.h                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/06/20 15:55:20 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/05/24 12:57:13 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATION_H
# define VALIDATION_H

int	isword(int c);
int	ispipe(int c);
int	isleftchevron(int c);
int	isrightchevron(int c);
int	isquote(int c);
int	isdquote(int c);
int	isinsidequote(int c);
int	isinsidedquote(int c);

int	istilde(int c);
int	isdollarsign(int c);
int	isvarstart(int c);
int	isinsidevar(int c);
int	isdigit_arg(int c);
int	iswordincompound(int c);
int	isquestionmark(int c);
int	ishereword(int c);

#endif
