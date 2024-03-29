/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:29:07 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/29 15:11:48 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**search_env(char *str, char **env)
{
	int	i;
	int	equals;

	i = 0;
	while (env[i])
	{
		equals = ft_strchr_pos(env[i], '=');
		if (equals != -1)
		{
			if (ft_strncmp(env[i], str, equals) == 0)
				return (&env[i]);
		}
		i++;
	}
	return (NULL);
}

int	del_from_env(char *str, char ***env)
{
	int		i;
	int		j;
	char	**new_env;

	new_env = malloc(sizeof(char *) * (ft_arrlen(*env) + 1));
	if (!new_env)
		return (write(2, MALLOC_ERROR, ft_strlen(MALLOC_ERROR)), -1);
	i = 0;
	j = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], str, ft_strlen(str)))
		{
			new_env[j] = ft_strdup((*env)[i]);
			if (!new_env[j])
				return (free(str), free_arr(new_env), -1);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	free_arr(*env);
	*env = new_env;
	return (0);
}

int	is_in_env(char *s, char **env)
{
	int	i;
	int	j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] == s[j] && s[j] != '=')
			j++;
		if ((env[i][j] == '\0' || env[i][j] == '=') && s[j] == '=')
			return (2);
		if ((env[i][j] == '\0' && s[j] == '\0')			//ca sert a rien ?
			|| (env[i][j] == '=' && s[j] == '\0'))
			return (1);
		i++;
	}
	return (0);
}

int	replace_in_env(char *str, char ***env)
{
	int	i;
	int	j;

	i = 0;
	while ((*env)[i])
	{
		j = 0;
		while ((*env)[i][j] && (*env)[i][j] == str[j] && str[j] != '=')
			j++;
		if ((*env)[i][j] == '=' || (*env)[i][j] == '\0')
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup(str);
			if (!(*env)[i])
				return (-1);
		}
		i++;
	}
	return (0);
}

char	**sort_env(char **env)
{
	int	i;
	int	j;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[j])
		{
			if (i != j && ft_strcmp(env[i], env[j]) < 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
	return (env);
}


