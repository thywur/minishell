/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 13:05:16 by quteriss          #+#    #+#             */
/*   Updated: 2024/03/08 13:40:19 by quteriss         ###   ########.fr       */
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