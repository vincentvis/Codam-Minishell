/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   status.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/07/15 16:40:58 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/08 09:30:31 by vincent       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUS_H
# define STATUS_H

# define EXIT_CODE_NOT_FOUND 127
# define EXIT_CODE_CANNOT_EXECUTE 126

enum e_status_code {
	SUCCESS = 0,
	BAD_INPUT,
	MALLOC_FAILURE,
	SHEENV_BAD_INPUT,
	SHEENV_DOUBLE_INIT,
	SHEENV_UNINITIALIZED,
	SHEENV_KEY_NOT_FOUND,
	L_BAD_INPUT,
	P_BAD_INPUT,
	E_FILE_FAILURE,
	E_FD_DUPLICATION_FAILURE,
	E_PIPE_FAILURE,
	E_FORK_FAILURE,
	E_CLOSE_FAILURE,
	E_COMMAND_NOT_FOUND,
	E_AMBIGUOUS_REDIRECT,
	E_HERE_FILE_FAILURE,
	E_HERE_FAILURE,
	WRITE_FAILURE,
	STATUS_CODE_MAX,
};

typedef struct s_response
{
	void				*data;
	enum e_status_code	status;	
}	t_response;

typedef int	(*t_error_handler)(const enum e_status_code status_code,
	void *reference, const char *msg);

t_response	make_response(int status, void *data);

int			manage_error(const int status_code, void *reference);

void		error_and_exit(const char *prefix, const char *msg, int errorcode);
void		perror_and_exit(const char *cmd);

int			log_error(
				const enum e_status_code status_code,
				void *reference,
				const char *msg);
int			log_perror(
				const enum e_status_code status_code,
				void *reference,
				const char *msg);
int			log_error_with_token(
				const enum e_status_code status_code,
				void *reference,
				const char *msg);
int			log_ambiguous_redirect_error(
				const enum e_status_code status_code,
				void *reference,
				const char *msg);
#endif
