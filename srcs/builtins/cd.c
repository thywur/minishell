/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:31:02 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/21 17:57:50 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_home(char **env)
{
	char	*home;
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("minishell");
		return (1);
	}
	home = getenv("HOME");
	if (!home)
	{
		write(2, "minishell: cd: HOME not set\n", 28);
		return (1);
	}
	if (chdir(home) == -1)
	{
		perror("minishell");
		return (1);
	}
	

}

int	ft_cd(char **args, char **env)
{
	char	*oldpwd;

	if (ft_arrlen(args) > 2)
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		return (1);
	}
	if (ft_arrlen(args) == 2)
	{

	}
}
