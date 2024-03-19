/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quentinterisse <quentinterisse@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 10:29:14 by quentinteri       #+#    #+#             */
/*   Updated: 2024/03/18 15:51:33 by quentinteri      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_block	*new_empty_block()
{
	t_block	*block;

	block = malloc(sizeof(t_block));
	if (!block)
		return (NULL);
	block->cmd = NULL;
	block->args = NULL;
	block->next = NULL;
	return (block);
}

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
		free(redir);
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
		while (elem->args && elem->args[i])
			free(elem->args[i++]);
		if (elem->args)
			free(elem->args);
		free_redir(elem->redir);
		free(elem);
		elem = next;
	}
	*blocks = NULL;
}

t_redir	*create_redir(t_token *token)
{
	return (NULL);
}

t_token	*create_block(t_block *block, t_token **tokens)
{
	/*
	
	- stocker token->data dans block->cmd (strdup)
	- head = *tokens
	- parcourir depuis head tant que je ne suis pas sur un |
		- si je suis sur un WORD
			- count++
		- sinon
			- je récupère le token suivant et je le push_back dans block->redir
			- je précise dans la redir le type de redirection (token->type)
	- block->args = malloc(sizeof(char *) * (count + 1));
	- je reparcours les token depuis head tant que je ne suis pas sur un |
		- si je suis sur un WORD
			- je l'ajoute a block->args
		
	*/

	t_token	*token;
	int		count;
	int		j;

	count = 1;
	token = *tokens;
	block->cmd = ft_strdup(token->data);
	if (!block->cmd)
		return (print_error(MALLOC_ERROR), NULL);
	token = token->next;
	while (token && token->next && token->type != PIPE)
	{
		if (token->type == WORD)
			count++;
		else if (token->type > 0 && token->type < 5)
		{
			block->redir = create_redir(token);
			if (!block->redir)
				return (print_error(MALLOC_ERROR), NULL);
			token = token->next;
		}
		token = token->next;
	}
	block->args = malloc(sizeof(char *) * (count + 1));
	if (!block->args)
		return (print_error(MALLOC_ERROR), NULL);
	block->args[count] = NULL;
	j = 0;
	token = *tokens;
	while (token && token->next && token->type != PIPE)
	{
		if (token->type == WORD)
		{
			block->args[j] = ft_strdup(token->data);
			if (!block->args[j])
				return (print_error(MALLOC_ERROR), NULL);
			j++;
		}
		token = token->next;
	}
	return (token);
}

t_block	*join_tokens_into_blocks(t_token **tokens)
{
	t_block	*block;
	t_token	*token;
    t_block	*head;

	block = new_empty_block();
	if (!block)
		return (print_error(MALLOC_ERROR), NULL);
	head = block;
	token = *tokens;
	while (token->next)
	{
		token = create_block(block, &token);
		if (!token)
			return (free_tokens(tokens), free_blocks(&head), NULL);
		block = new_empty_block();
		if (!block)
			return (free_tokens(tokens), free_blocks(&head), print_error(MALLOC_ERROR), NULL);
		token = token->next;
	}
	block = head;
	free_tokens(tokens);
	return (block);
}
