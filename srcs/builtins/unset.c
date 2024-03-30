/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 17:33:26 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/25 17:43:59 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_unset(char *s)
{
	int	i;

	i = 0;
	while (s[i] && (ft_isalpha(s[i]) || s[i] != '_')
		&& s[i] != '=' && s[i] != '\n')
		i++;
	if (s[i] == '\0' || s[i] == '=')
		return (1);
	write(2, "minishell: unset: '", 19);
	write(2, s, ft_strlen(s));
	write(2, "': not a valid identifier\n", 26);
	return (0);
}

int	ft_unset(char **args, char ***env)
{
	int	i;
	int	exit_status;

	if (ft_arrlen(args) == 1)
		return (0);
	i = 1;
	exit_status = 0;
	while (args[i])
	{
		if (!is_valid_unset(args[i]))
			exit_status = 1;
		else
		{
			if (del_from_env(args[i], env) == -1)
				exit_status = 1;
		}
		i++;
	}
	return (exit_status);
}
