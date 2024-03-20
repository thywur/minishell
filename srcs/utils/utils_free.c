/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:53:33 by quteriss          #+#    #+#             */
/*   Updated: 2024/03/19 14:57:07 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redir(t_redir **redir)
{
	t_redir	*next;
	t_redir	*elem;

	elem = *redir;
	while (elem)
	{
		next = elem->next;
		if (elem->file)
			free(elem->file);
		free(elem);
		elem = next;
	}
	*redir = NULL;
}

void	free_blocks(t_block **blocks)
{
	t_block	*next;
	t_block	*elem;
	int		i;

	elem = *blocks;
	while (elem)
	{
		next = elem->next;
		if (elem->cmd)
			free(elem->cmd);
		i = 0;
		while (elem->args && elem->args[i])
			free(elem->args[i++]);
		if (elem->args)
			free(elem->args);
		free_redir(&elem->redir);
		free(elem);
		elem = next;
	}
	*blocks = NULL;
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
