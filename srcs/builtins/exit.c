/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 16:27:08 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/19 13:56:05 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	arg_is_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	ft_atoi_exit(char *str)
{
	unsigned long long	nb;
	int					sign;
	int					res;

	nb = 0;
	sign = 1;
	res = 0;
	while(str[res] == ' ' || (str[res] >= 9 && str[res] <= 13))
		res++;
	if (str[res] == '-' || str[res] == '+')
	{
		if (str[res] == '-')
			sign = -1;
		res++;
	}
	while (str[res] >= '0' && str[res] <= '9')
		nb = nb * 10 + (str[res++] - '0');
	if (nb > LLONG_MAX)
		return (-1);
	res = nb % 256;
	if (sign == -1)
		res = res * -1 + 256;
	return (res);
}

int	ft_exit(char **args)
{
	if (ft_arrlen(args) > 1)
	{
		if (!arg_is_numeric(args[1]) || ft_atoi_exit(args[1] == -1))
		{
			write(2, "exit\nminishell: exit: ", 22);
			write(2, args[1], ft_strlen(args[1]));
			write(2, ": numeric argument required\n", 28);
			exit(2);
		}
		else if (ft_arrlen(args) == 2)
			exit(ft_atoi_exit(args[1]));
		if (ft_arrlen(args) > 2)
			write(2, "exit\nminishell: exit: too many arguments\n", 41);
		return (1);
	}
	exit(0);
	return (0);
}
