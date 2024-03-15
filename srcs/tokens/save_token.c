/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 13:07:28 by quteriss          #+#    #+#             */
/*   Updated: 2024/03/15 13:57:28 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*save_word(t_token *token, char *cmdline, int *last_pos, int pos)
{
	if (is_not_empty(cmdline + *last_pos, pos - *last_pos))
	{
		token->type = WORD;
		token->data = ft_strdup_size(cmdline + *last_pos, pos - *last_pos);
		if (!token->data)
			return (NULL);
		token = add_token(token);
		if (!token)
			return (NULL);
	}
	*last_pos = pos;
	return (token);
}

t_token	*save_symbol(t_token *token, char *symbol, int *last_pos, int *pos)
{
	int	size;

	size = ft_strlen(symbol);
	if (!ft_strcmpr(symbol, "|"))
		token->type = PIPE;
	else if (size == 2)
		token->type = (REDIRECT_APPEND * (symbol[0] == '>'))
			+ (REDIRECT_HEREDOC * (symbol[0] == '<'));
	else
		token->type = (REDIRECT_OUT * (symbol[0] == '>'))
			+ (REDIRECT_IN * (symbol[0] == '<'));
	token->data = ft_strdup_size(symbol, size);
	if (!token->data)
		return (NULL);
	token = add_token(token);
	if (!token)
		return (NULL);
	*last_pos = *pos + 1 + (size == 2);
	*pos += (size == 2);
	return (token);
}

t_token	*save_token(t_token *token, char *cmdline, t_token_args *args)
{
	if (ft_contains("\"'", cmdline[args->pos]) && !args->quote)
		args->quote = cmdline[args->pos];
	else if (cmdline[args->pos] && args->quote == cmdline[args->pos])
		args->quote = 0;
	if ((ft_contains(" |<>", cmdline[args->pos]) || cmdline[args->pos] == '\0')
		&& !args->quote)
		token = save_word(token, cmdline, &args->last_pos, args->pos);
	if (!token)
		return (NULL);
	if (cmdline[args->pos] == '|' && !args->quote)
		token = save_symbol(token, "|", &args->last_pos, &args->pos);
	else if (!args->quote && ft_startswith(cmdline + args->pos, "<<"))
		token = save_symbol(token, "<<", &args->last_pos, &args->pos);
	else if (!args->quote && ft_startswith(cmdline + args->pos, ">>"))
		token = save_symbol(token, ">>", &args->last_pos, &args->pos);
	else if (!args->quote && cmdline[args->pos] == '>')
		token = save_symbol(token, ">", &args->last_pos, &args->pos);
	else if (!args->quote && cmdline[args->pos] == '<')
		token = save_symbol(token, "<", &args->last_pos, &args->pos);
	return (token);
}
