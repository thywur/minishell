/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:40:38 by alermolo          #+#    #+#             */
/*   Updated: 2024/01/02 15:01:44 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 10000000
#endif

void	*free_and_null(char *s)
{
	free(s);
	return (NULL);
}

static int	find_endl(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

static void	push_buffer(char *buffer)
{
	int	i;
	int	j;

	if (find_endl(buffer) == -1)
		i = BUFFER_SIZE;
	else
	{
		if (buffer[0] == '\n' && buffer[1] == '\0')
			buffer[0] = '\0';
		i = find_endl(buffer) + 1;
	}
	j = 0;
	while (buffer[i] && i < BUFFER_SIZE)
		buffer[j++] = buffer[i++];
	if (j < i)
		buffer[j] = '\0';
	else
		buffer[i] = '\0';
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*line;
	ssize_t		read_size;

	if (read(fd, 0, 0) == -1)
		ft_bzero(buffer, BUFFER_SIZE);
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) == -1)
		return (NULL);
	line = NULL;
	push_buffer(buffer);
	line = ft_strjoin_free(line, buffer);
	while (find_endl(buffer) == -1)
	{
		read_size = read(fd, buffer, BUFFER_SIZE);
		if (read_size == -1)
			return (NULL);
		buffer[read_size] = '\0';
		if (read_size == 0)
			return (line);
		line = ft_strjoin_free(line, buffer);
	}
	if (find_endl(buffer) >= 0)
		line = ft_substr_gnl(line, 0, find_endl(line) + 1);
	return (line);
}
