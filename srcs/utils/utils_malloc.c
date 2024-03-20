/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 11:53:11 by quteriss          #+#    #+#             */
/*   Updated: 2024/03/19 14:54:36 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup_size(char *str, int size)
{
	char	*new_str;
	int		i;

	new_str = malloc(sizeof(char) * (size + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < size && str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

void	ft_lst_delone(t_token **tokens, int index)
{
	t_token	*prev_elem;
	t_token	*elem;
	int		i;

	i = 0;
	elem = *tokens;
	prev_elem = NULL;
	while (i < index && elem)
	{
		prev_elem = elem;
		elem = elem->next;
		i++;
	}
	prev_elem->next = elem->next;
	free(elem->data);
	free(elem);
}
