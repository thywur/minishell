/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quentinterisse <quentinterisse@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 11:47:46 by quteriss          #+#    #+#             */
/*   Updated: 2024/03/18 15:26:46 by quentinteri      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*add_token(t_token *token)
{
	t_token	*next_token;

	next_token = malloc(sizeof(t_token));
	if (!next_token)
		return (NULL);
	next_token->next = NULL;
	next_token->data = NULL;
	next_token->type = 0;
	token->next = next_token;
	return (next_token);
}

t_token	*create_empty_token(void)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->next = NULL;
	token->data = NULL;
	token->type = 0;
	return (token);
}

void	trim_token_data(t_token *token)
{
	char	*trimed_data;

	trimed_data = ft_strtrim(token->data, " \t");
	free(token->data);
	token->data = trimed_data;
}
