/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 16:21:23 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/30 16:07:29 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_export(char *s)
{
	int	i;

	i = 0;
	while (s[i] && (ft_isalpha(s[i]) || s[i] != '_')
		&& s[i] != '=' && s[i] != '\n')
		i++;
	if (s[i] == '\0' || s[i] == '=')
		return (1);
	write(2, "minishell: export: '", 20);
	write(2, s, ft_strlen(s));
	write(2, "': not a valid identifier\n", 26);
	return (0);
}

int	add_to_env(char *str, char ***env)
{
	int		i;
	char	**new_env;

	new_env = malloc(sizeof(char *) * (ft_arrlen(*env) + 2));
	if (!new_env)
	{
		write(2, MALLOC_ERROR, ft_strlen(MALLOC_ERROR));
		return (1);
	}
	i = 0;
	while ((*env)[i])
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
	return (0);
}

static int	print_declare_env(char **env, int fd[2])
{
	int	i;
	int	end;

	env = sort_env(env);
	i = 0;
	while (env[i])
	{
		write(fd[1], "declare -x ", 11);
		end = ft_strchr_pos(env[i], '=');
		if (end == -1)
			write(fd[1], env[i], ft_strlen(env[i]));
		else
		{
			write(fd[1], env[i], end + 1);
			write(fd[1], "\"", 1);
			write(fd[1], &env[i][end + 1], ft_strlen(env[i]) - end - 1);
			write(fd[1], "\"", 1);
		}
		write(fd[1], "\n", 1);
		i++;
	}
	return (0);
}

int	ft_export(char **args, char ***env, int fd[2])
{
	int	i;
	int	exit_status;

	if (ft_arrlen(args) == 1)
		return (print_declare_env(*env, fd));
	i = 1;
	exit_status = 0;
	while (args[i])
	{
		if (!is_valid_export(args[i]))
			exit_status = 1;
		else if (is_in_env(args[i], *env) == 2)
		{
			if (replace_in_env(args[i], env) == -1)
				return (EXIT_FAILURE);
		}
		else if (!is_in_env(args[i], *env))
		{
			if (add_to_env(args[i], env) == -1)
				return (EXIT_FAILURE);
		}
		i++;
	}
	return (exit_status);
}
