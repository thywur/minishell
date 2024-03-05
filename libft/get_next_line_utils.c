/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:25:40 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/05 12:00:52 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		size;
	int		i;
	char	*dest;

	size = ft_strlen(s);
	i = 0;
	dest = malloc(sizeof(char) * (size + 1));
	if (!dest)
		return (NULL);
	while (i < size)
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_substr_gnl(char *s, unsigned int start, size_t len)
{
	char			*new;
	unsigned long	i;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
	{
		len = 0;
		start = ft_strlen(s);
	}
	else if (start + len > ft_strlen(s) || len > ft_strlen(s))
		len = ft_strlen(s) - start;
	new = (char *)malloc(sizeof(char) * (len + 1));
	if (!new)
		return (free_and_null(s));
	i = 0;
	while (s[start] && i < len)
		new[i++] = s[start++];
	new[i] = '\0';
	free(s);
	return (new);
}

char	*ft_strjoin_free(char const *s1, char const *s2)
{
	char	*str;
	int		i;
	int		j;

	if ((!s1 && !s2) || (!s1 && s2[0] == '\0'))
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return ((char *)s1);
	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (free_and_null((void *) s1));
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		str[i++] = s2[j++];
	str[i] = '\0';
	free((void *) s1);
	return (str);
}

void	*ft_bzero(void *s, size_t n)
{
	unsigned char	*src;
	unsigned long	i;

	if (!s)
		return (NULL);
	src = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		src[i] = 0;
		i++;
	}
	return (s);
}
