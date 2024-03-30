/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 14:07:06 by quteriss          #+#    #+#             */
/*   Updated: 2024/03/30 14:20:29 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unexpected_token_error(char	*data)
{
	char	*error_message;

	error_message = ft_strjoin("syntax error near unexpected token ", data);
	print_error(error_message);
	free(error_message);
}

int	check_tokens(t_token **token)
{
	t_token	*elem;

	elem = *token;
	while (elem)
	{
		if (elem->type == PIPE && elem->next && elem->next->type == PIPE)
			return (unexpected_token_error("|"), 0);
		if (elem->type == PIPE && !elem->next)
			return (unexpected_token_error("newline"), 0);
		if (elem->type >= 1 && elem->type <= 4 && (!elem->next || elem->next->type != WORD))
		{
			if (elem->next && elem->next->data)
				return (unexpected_token_error(elem->next->data), 0);
			else
				return (unexpected_token_error("newline"), 0);
		}
		elem = elem->next;
	}
	return (1);
}

/*

[ ]	echo helloworld || pwd
[ ] < | fuck
[ ] |

*/
