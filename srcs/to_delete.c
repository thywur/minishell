/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_delete.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 15:27:02 by quentinteri       #+#    #+#             */
/*   Updated: 2024/03/21 16:13:21 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void    print_blocks(t_block **blocks)
{
	t_block	*elem;
	t_redir	*redir;
	int		i;
	int		j;

	i = 1;
	elem = *blocks;
	while (elem)
	{
		printf("block%d (%p) { cmd:%s, args: ", i, elem, elem->cmd);
		j = 0;
		while (elem->args[j])
		{
			printf("%s%c", elem->args[j], ';' * (elem->args[j + 1] != NULL));
			j++;
		}
		printf(", redir: ");
		redir = elem->redir;
		while (redir)
		{
			printf("%s(%d)%c", redir->file, redir->type, ';' * (redir->next != NULL));
			redir = redir->next;
		}
		printf(", next: %p}\n", elem->next);
		elem = elem->next;
		i++;
	}
}
