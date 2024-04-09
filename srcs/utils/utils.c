/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 13:05:16 by quteriss          #+#    #+#             */
/*   Updated: 2024/04/09 13:47:32 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(char *error_descriptor)
{
	int	i;

	i = 0;
	write(2, "minishell: ", 11);
	while (error_descriptor[i])
	{
		write(2, error_descriptor + i, 1);
		i++;
	}
	write(2, "\n", 1);
}

void	get_final_data_size_extra(char *quote, char car, int *size)
{
	if (!*quote && ft_contains("\"'", car))
		*quote = car;
	else if (*quote && *quote == car)
		*quote = 0;
	else
		(*size)++;
}

void	expand_token_extra(char *quote, char car, char *word, int *size)
{
	if (!*quote && ft_contains("\"'", car))
		*quote = car;
	else if (*quote && *quote == car)
		*quote = 0;
	else
	{
		word[*size] = car;
		(*size)++;
	}
}

int	is_ascii(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] >= 32 && str[i] <= 126)
		i++;
	return (str[i] == '\0');
}
