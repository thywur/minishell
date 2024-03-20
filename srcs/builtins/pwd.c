/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 16:01:18 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/14 16:05:38 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(int fd[2])
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		write(2, "minishell: pwd: error retrieving current directory\n", 51);
		return (1);
	}
	write(fd[1], pwd, ft_strlen(pwd));
	write(fd[1], "\n", 1);
	free(pwd);
	return (0);
}
