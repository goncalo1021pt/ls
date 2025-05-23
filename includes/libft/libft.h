/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: goncalo1021pt <goncalo1021pt@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:02:58 by gfontao-          #+#    #+#             */
/*   Updated: 2025/05/22 12:32:18 by goncalo1021      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# define EX_LOW_BASE "0123456789abcdef"
# define EX_UP_BASE "0123456789ABCDEF"
# define PRINTF_FLAGS "cspdiuxX%"
# define BUFFER_SIZE 10

# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <stdio.h>

int				ft_printf(const char *str, ...);
int				ft_putnbr_base(unsigned int nbr, char *base, int total);
int				ft_putpt(unsigned long int nbr, char *base, int total);
int				ft_putnbr(int n, int total);
int				ft_putunbr(unsigned int n, int total);
int				ft_atoi(const char *nptr);
void			ft_bzero(void *str, size_t n);
void			*ft_calloc(size_t nmemb, size_t size);
int				ft_isalnum(int c);
int				ft_isalpha(int c);
int				ft_isascii(int c);
int				ft_isdigit(int c);
int				ft_isprint(int c);
char			*ft_itoa(int n);
void			*ft_memccpy(void *dest, const void *src, int c, size_t n);
void			*ft_memchr(const void *str, int c, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
void			*ft_memcpy(void *dest, const void *src, size_t n);
void			*ft_memmove(void *dest, const void *src, size_t n);
void			*ft_memset(void *str, int c, size_t n);
int				ft_putchar_fd(char c, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_putnbr_fd(int n, int fd);
int				ft_putstr_fd(char *s, int fd);
char			**ft_split(char const *s, char c);
void			ft_free_split(char **split);
char			*ft_strchr(const char *s, int c);
char			*ft_strdup(const char *s);
void			ft_striteri(char *s, void (*f)(unsigned int, char*));
char			*ft_strjoin(char const *s1, char const *s2);
unsigned int	ft_strlcat(char *dest, char *src, unsigned int size);
size_t			ft_strlcpy(char *dest, const char *src, size_t size);
size_t			ft_strlen(const char *str);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*ft_strnstr(char *str, const char *to_find, size_t len);
char			*ft_strrchr(const char *s, int c);
char			*ft_strtrim(char const *s1, char const *set);
char			*ft_substr(char const *s, unsigned int start, size_t len);
int				ft_tolower(int c);
int				ft_toupper(int c);
int				ft_linelen(char *str);
char			*ft_join_line(char *s1, char *s2);
void			line_buffer_clean(char *str);
char			*get_next_line(int fd);
long			ft_atoi_long(char *nptr);
char			*ft_strcpy(char *dest, char *src);
char			*ft_strncpy(char *dest, char *src, unsigned int n);

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

void			ft_lstadd_back(t_list **lst, t_list *new);
void			ft_lstadd_front(t_list **lst, t_list *new);
void			ft_lstclear(t_list **lst, void (*del)(void *));
void			ft_lstdelone(t_list *lst, void (*del)(void *));
void			ft_lstiter(t_list *lst, void (*f)(void *));
t_list			*ft_lstlast(t_list *lst);
t_list			*ft_lstmap(t_list *lst, void *(*f)(void *),
					void (*del)(void *));
t_list			*ft_lstnew(void *content);
int				ft_lstsize(t_list *lst);

// fprintf
int				ft_putnbr_base_fd(int fd, unsigned int nbr,
					char *base, int total);
int				ft_putpt_fd(int fd, unsigned long int nbr,
					char *base, int total);
int				ft_putnbrt_fd(int fd, int n, int total);
void			ft_putlnbr_fd(long int n, int fd);
int				ft_putunbr_fd(int fd, unsigned int n, int total);
int				ft_fprintf(int fd, const char *str, ...);

// new functions
void			*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char			*ft_strncat(char *dest, const char *src, size_t n);
int				ft_strcmp(const char *s1, const char *s2);
int				ft_isupper(int c);
int				ft_islower(int c);

#endif