/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 15:02:04 by quteriss          #+#    #+#             */
/*   Updated: 2024/04/04 17:04:41 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstadd_back(t_redir **lst, t_redir *new)
{
	t_redir	*elem;

	elem = *lst;
	if (!elem)
	{
		*lst = new;
		return ;
	}
	while (elem->next)
		elem = elem->next;
	elem->next = new;
}

t_redir	*create_redir(t_token *token, t_token *file_token, int *heredoc)
{
	t_redir	*redir;

	*heredoc = 1;
	if (file_token->is_inquote)
		*heredoc = 0;
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->file = ft_strdup(file_token->data);
	if (!redir->file)
		return (NULL);
	redir->type = token->type;
	redir->next = NULL;
	return (redir);
}
