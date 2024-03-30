/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 11:51:45 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/28 11:25:17 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

char	**ft_split(char *str, char *charset);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	ft_strlen(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoin_free(char const *s1, char const *s2);
char	*ft_strjoin_free2(char const *s1, char const *s2);
char	*get_next_line(int fd);
void	*free_and_null(char *s);
char	*ft_strdup(const char *s);
char	*ft_substr_gnl(char *s, unsigned int start, size_t len);
void	*ft_bzero(void *s, size_t n);
int		ft_isdigit(int c);
int		ft_arrlen(char **arr);
char	*ft_strstr(const char *big, const char *little);
char	*ft_itoa(int n);
// int		ft_lstsize(t_block *lst);
// t_redir	*ft_lstlast(t_redir *lst);

#endif
