/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   libft.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: vincent <vincent@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/20 09:44:55 by vvissche      #+#    #+#                 */
/*   Updated: 2022/06/07 16:38:58 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include "ft_nbr.h"
# include "ft_bigint.h"

size_t					ft_strlen(const char *str);
size_t					ft_strnlen(const char *str, size_t maxlen);
size_t					ft_wcslen(const int *str);
size_t					ft_wcsnlen(const int *str, size_t maxlen);
char					*ft_strchr(const char *s, int c);
char					*ft_strrchr(const char *s, int c);
size_t					ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t					ft_strlcpy(char *dst, const char *src, size_t size);
char					*ft_strnstr(const char *hay, const char *ndl,
							size_t len);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
int						ft_atoi(const char *str);
int						ft_atoi_chars(const char *str);
int						ft_isalpha(int c);
int						ft_isupper(int c);
int						ft_islower(int c);
int						ft_isdigit(int c);
int						ft_isalnum(int c);
int						ft_isprint(int c);
int						ft_isascii(int c);
int						ft_toupper(int c);
int						ft_tolower(int c);
void					*ft_memset(void *b, int c, size_t len);
void					*ft_memset_int(void *b, unsigned int c, size_t len);
void					ft_bzero(void *s, size_t n);
void					*ft_memcpy(void *dst, const void *src, size_t n);
void					*ft_memccpy(void *dest, const void *src, int c,
							size_t n);
void					*ft_memmove(void *dest, const void *src, size_t n);
void					*ft_memchr(const void *s, int c, size_t n);
int						ft_memcmp(const void *s1, const void *s2, size_t n);
void					*ft_calloc(size_t count, size_t size);
char					*ft_strdup(const char *s1);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
char					*ft_strjoin(char const *s1, char const *s2);
char					*ft_strtrim(char const *s1, char const *set);
char					**ft_split(char const *s, char c);
char					*ft_itoa(int n);
char					*ft_strmapi(char const *s,
							char (*f)(unsigned int, char));
void					ft_putchar_fd(char c, int fd);
void					ft_putstr_fd(char *s, int fd);
void					ft_putendl_fd(const char *s, int fd);
void					ft_putnbr_fd(int n, int fd);

/*
** BONUS
*/

typedef struct s_list
{
	void				*content;
	struct s_list		*next;
}						t_list;
t_list					*ft_lstnew(void *content);
void					ft_lstadd_front(t_list **alst, t_list *new);
int						ft_lstsize(t_list *lst);
t_list					*ft_lstlast(t_list *lst);
void					ft_lstadd_back(t_list **alst, t_list *new);
void					ft_lstdelone(t_list *lst, void (*del)(void *));
void					ft_lstclear(t_list **lst, void (*del)(void *));
void					ft_lstiter(t_list *lst, void (*f)(void *));
t_list					*ft_lstmap(t_list *lst, void *(*f)(void *),
							void (*del)(void *));
/*
** PDF page 12: You are free to add any function to your libft as you see fit.
*/

uint_fast8_t			ft_nbrlen(long n);
uint_fast8_t			ft_nbrlen_base(long value, long base);
uint_fast8_t			ft_nbrlen_unsigned(unsigned long value);
uint_fast8_t			ft_nbrlen_unsigned_base(unsigned long value,
							unsigned long base);
uint_fast8_t			ft_nbrlen_unsigned_base16(unsigned long value);
uint_fast8_t			ft_nbrlen_unsigned_base8(unsigned long value);
uint_fast8_t			ft_nbrlen_unsigned_base2(unsigned long value);
int						ft_isspace(int c);
int						ft_is_null(const void *ptr);
void					ft_segdefault(void);
void					*ft_memdup(const char *s1, size_t n);
long					ft_strtol(const char *str, char **endptr, int base);
long long				ft_strtoll(const char *str, char **endptr, int base);
void					free_char_pointer_array(char **array);

#endif
