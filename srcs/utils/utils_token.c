/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 11:47:46 by quteriss          #+#    #+#             */
/*   Updated: 2024/03/15 13:58:58 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// A SUPPRIMER !!!!
void	print_tokens(t_token **tokens)
{
	t_token	*elem;
	int		i;

	i = 1;
	elem = *tokens;
	while (elem)
	{
		printf("token%d (%p) { type:%d, data:%s, next:%p }\n", i, elem,
			elem->type, elem->data, elem->next);
		elem = elem->next;
		i++;
	}
}

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
