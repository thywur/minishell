/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 11:44:31 by quteriss          #+#    #+#             */
/*   Updated: 2024/03/08 13:38:18 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmpr(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	ft_contains(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (str[i] == c);
}

int	ft_startswith(char *big, char *little)
{
	int	i;

	i = 0;
	while (big[i] && little[i] && big[i] == little[i])
		i++;
	return (little[i] == '\0');
}

int	is_not_empty(char *str, int size)
{
	int	i;

	i = 0;
	while (i < size && str[i] && ft_contains(" \f\n\r\t\v", str[i]))
		i++;
	return (str[i] != '\0' && i != size);
}
