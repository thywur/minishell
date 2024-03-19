/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quentinterisse <quentinterisse@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 11:53:11 by quteriss          #+#    #+#             */
/*   Updated: 2024/03/18 13:30:27 by quentinteri      ###   ########.fr       */
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

void	free_tokens(t_token **tokens)
{
	t_token	*next;
	t_token	*elem;

	elem = *tokens;
	while (elem)
	{
		next = elem->next;
		if (elem->data)
			free(elem->data);
		free(elem);
		elem = next;
	}
	*tokens = NULL;
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
