/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 16:11:42 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/19 14:13:46 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_n(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(char **args, int fd[2])
{
	int	i;
	int	has_newline;

	i = 1;
	has_newline = 0;
	if (ft_arrlen(args) == 1)
	{
		write(fd[1], "\n", 1);
		return (0);
	}
	while (args[i] && args[i][0] == '-' && is_n(&args[i][1]))
	{
		has_newline = 1;
		i++;
	}
	while (args[i])
	{
		write(fd[1], args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(fd[1], " ", 1);
		i++;
	}
	if (!has_newline)
		write(fd[1], "\n", 1);
	return (0);
}
