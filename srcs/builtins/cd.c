/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:31:02 by alermolo          #+#    #+#             */
/*   Updated: 2024/04/08 17:49:33 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_to_env_free(char *str, char ***env)
{
	int		i;
	char	**new_env;

	new_env = malloc(sizeof(char *) * (ft_arrlen(*env) + 2));
	if (!new_env)
	{
		write(2, MALLOC_ERROR, ft_strlen(MALLOC_ERROR));
		return (-1);
	}
	i = 0;
	while ((*env) && (*env)[i])
	{
		new_env[i] = ft_strdup((*env)[i]);
		if (!new_env[i])
			return (free(str), free_arr(new_env), -1);
		i++;
	}
	new_env[i] = ft_strdup(str);
	if (!new_env[i])
		return (free(str), free_arr(new_env), -1);
	new_env[i + 1] = NULL;
	free_arr(*env);
	*env = new_env;
	free(str);
	return (0);
}

int	cd_path(char *path, char ***env)
{
	char	*temp_pwd;

	temp_pwd = getcwd(NULL, 0);
	if (!temp_pwd)
		return (perror("minishell"), 1);
	if (chdir(path) == -1)
		return (free(temp_pwd), perror("minishell"), 1);
	if (del_from_env("OLDPWD", env) == -1)
		return (free(temp_pwd), perror("minishell"), 1);
	if (add_to_env_free(ft_strjoin_free2("OLDPWD=", temp_pwd), env) == -1)
		return (free(temp_pwd), perror("minishell"), 1);
	if (del_from_env("PWD", env) == -1)
		return (perror("minishell"), 1);
	if (add_to_env_free(ft_strjoin_free2("PWD=", getcwd(NULL, 0)), env) == -1)
		return (1);
	return (0);
}

int	cd_home(char ***env)
{
	char	*home;
	char	*oldpwd;

	home = getenv("HOME");
	if (!home)
	{
		write(2, "minishell: cd: HOME not set\n", 28);
		return (1);
	}
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (perror("minishell"), 1);
	if (chdir(home) == -1)
		return (free(oldpwd), perror("minishell"), 1);
	if (del_from_env("OLDPWD", env) == -1)
		return (free(oldpwd), 1);
	if (add_to_env_free(ft_strjoin_free2("OLDPWD=", oldpwd), env) == -1)
		return (1);
	if (del_from_env("PWD", env) == -1)
		return (1);
	if (add_to_env_free(ft_strjoin("PWD=", home), env) == -1)
		return (1);
	return (0);
}

int	ft_cd(char **args, char ***env)
{
	char	**oldpwd;

	if (ft_arrlen(args) > 2)
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		return (1);
	}
	if (ft_arrlen(args) == 2)
	{
		if (ft_strcmp(args[1], "~") == 0)
			return (cd_home(env));
		else if (ft_strcmp(args[1], "-") == 0)
		{
			oldpwd = search_env("OLDPWD", *env);
			if (!oldpwd)
				return (write(2, "minishell: cd: OLDPWD not set\n", 30), 1);
			return (cd_path((&(*oldpwd)[7]), env));
		}
		return (cd_path(args[1], env));
	}
	return (cd_home(env));
}
