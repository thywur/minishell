/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 10:29:14 by quentinteri       #+#    #+#             */
/*   Updated: 2024/03/27 13:42:04 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_block	*del_last(t_block **blocks)
{
	t_block	*block;
	t_block	*last;

	block = *blocks;
	while (block && block->next && block->next->next)
		block = block->next;
	if (block)
	{
		last = block->next;
		block->next = NULL;
		free_blocks(&last);
	}
	return (*blocks);
}

t_block	*new_empty_block(void)
{
	t_block	*block;

	block = malloc(sizeof(t_block));
	if (!block)
		return (NULL);
	block->cmd = NULL;
	block->args = NULL;
	block->next = NULL;
	block->redir = NULL;
	return (block);
}

t_token	*save_block_args(t_block *block, int count, t_token **tokens)
{
	t_token	*token;
	int		j;

	j = 0;
	block->args = malloc(sizeof(char *) * (count + 1));
	if (!block->args)
		return (print_error("malloc creation error"), NULL);
	token = *tokens;
	while (token && token->next && token->type != PIPE)
	{
		if (token->type == WORD)
		{
			block->args[j] = ft_strdup(token->data);
			if (!block->args[j])
				return (print_error("malloc creation error"), NULL);
			j++;
		}
		else
			token = token->next;
		token = token->next;
	}
	block->args[count] = NULL;
	return (token);
}

t_token	*create_block(t_block *block, t_token **tokens, int count)
{
	t_token	*token;
	t_redir	*redir;

	token = *tokens;
	while (token && token->next && token->type != PIPE)
	{
		if (token->type == WORD)
			count++;
		else if (token->type > 0 && token->type < 5)
		{
			redir = create_redir(token, token->next);
			if (!redir)
				return (NULL);
			ft_lstadd_back(&block->redir, redir);
			token = token->next;
		}
		token = token->next;
	}
	token = save_block_args(block, count, tokens);
	block->cmd = ft_strdup(block->args[0]);
	if (!block->cmd)
		return (NULL);
	return (token);
}

t_block	*join_tokens_into_blocks(t_token **tokens)
{
	t_block	*block;
	t_token	*token;
	t_block	*next;
	t_block	*head;

	block = new_empty_block();
	if (!block)
		return (NULL);
	head = block;
	token = *tokens;
	while (token && token->next)
	{
		token = create_block(block, &token, 0);
		if (!token)
			return (free_tokens(tokens), free_blocks(&head), NULL);
		next = new_empty_block();
		if (!next)
			return (free_tokens(tokens), free_blocks(&head), NULL);
		block->next = next;
		block = next;
		token = token->next;
	}
	block = head;
	free_tokens(tokens);
	return (del_last(&block));
}
