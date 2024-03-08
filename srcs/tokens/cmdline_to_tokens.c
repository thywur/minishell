/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline_to_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:00:47 by quteriss          #+#    #+#             */
/*   Updated: 2024/03/08 13:29:53 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*save_tokens(t_token *token, char *cmdline, t_token_args *args)
{
	while (42)
	{
		token = save_token(token, cmdline, args);
		if (!token)
			return (print_error("malloc creation error"), NULL);
		if (token->error == UNCLOSED_QUOTE)
			return (print_error("unclosed quote error"), NULL);
		if (cmdline[args->pos] == '\0')
			break ;
		args->pos++;
	}
	return (token);
}

t_token	*split_cmdline_into_tokens(char *cmdline)
{
	t_token			*tokens;
	t_token			*token;
	t_token_args	args;

	args.pos = 0;
	args.last_pos = 0;
	args.inside_quotes = 0;
	token = create_empty_token();
	if (!token)
		return (print_error("malloc creation error"), NULL);	
	tokens = token;
	token = save_tokens(token, cmdline, &args);
	if (!token)
		return (free_linked_array(&tokens), NULL);
	token = tokens;
	return (token);
}
