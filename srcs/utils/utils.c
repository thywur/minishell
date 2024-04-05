/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 13:05:16 by quteriss          #+#    #+#             */
/*   Updated: 2024/04/05 14:43:27 by quteriss         ###   ########.fr       */
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

void	expand_token_extra(char *quote, char car, char *word, int *j)
{
	if (!*quote && ft_contains("\"'", car))
		*quote = car;
	else if (*quote && *quote == car)
		*quote = 0;
	else
	{
		word[*j] = car;
		(*j)++;
	}
}
