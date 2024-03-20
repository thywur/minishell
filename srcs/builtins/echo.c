/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 16:11:42 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/14 16:18:40 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **args, int fd[2])
{
	int	i;
	int	has_newline;

	i = 1;
	has_newline = 0;
	if (args[i] && !ft_strncmp(args[i], "-n", 2))
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
