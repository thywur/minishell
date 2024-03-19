/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline_to_tokens.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quentinterisse <quentinterisse@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 18:00:47 by quteriss          #+#    #+#             */
/*   Updated: 2024/03/18 13:30:52 by quentinteri      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*save_tokens(t_token *token, char *cmdline, t_token_args *args)
{
	while (42)
	{
		token = save_token(token, cmdline, args);
		if (!token)
			return (print_error(MALLOC_ERROR), NULL);
		if (cmdline[args->pos] == '\0')
			break ;
		args->pos++;
	}
	if (args->quote)
		return (print_error("unclosed quote error"), NULL);
	return (token);
}

t_token	*split_cmdline_into_tokens(char *cmdline)
{
	t_token			*tokens;
	t_token			*token;
	t_token_args	args;

	args.pos = 0;
	args.quote = 0;
	args.last_pos = 0;
	token = create_empty_token();
	if (!token)
		return (print_error(MALLOC_ERROR), NULL);
	tokens = token;
	token = save_tokens(token, cmdline, &args);
	if (!token)
		return (free_tokens(&tokens), NULL);
	token = tokens;
	return (token);
}
