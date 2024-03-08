/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 13:07:28 by quteriss          #+#    #+#             */
/*   Updated: 2024/03/08 13:37:23 by quteriss         ###   ########.fr       */
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

t_token	*save_quotes(t_token *token, char *cmdline, int *last_pos, int *pos)
{
	char	c;

	*last_pos = *pos;
	c = cmdline[(*pos)++];
	while (cmdline[*pos] && cmdline[*pos] != c)
		(*pos)++;
	if (cmdline[*pos] == '\0')
	{
		token->error = UNCLOSED_QUOTE;
		return (token);
	}
	token->type = WORD;
	token->data = ft_strdup_size(cmdline + *last_pos, (*pos - *last_pos) + 1);
	if (!token->data)
		return (NULL);
	token = add_token(token);
	if (!token)
		return (NULL);
	*last_pos = *pos + 1;
	return (token);
}

t_token	*save_symbol(t_token *token, char *symbol, int *last_pos, int *pos)
{
	int	size;

	size = ft_strlen(symbol);
	if (!ft_strcmpr(symbol, "|"))
		token->type = PIPE;
	else if (size == 2)
		token->type = (REDIRECT_APPEND * (symbol[0] == '>')) +
			(REDIRECT_HEREDOC * (symbol[0] == '<'));
	else
		token->type = (REDIRECT_OUT * (symbol[0] == '>')) +
			(REDIRECT_IN * (symbol[0] == '<'));
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
	if ((ft_contains(" |<>", cmdline[args->pos]) || cmdline[args->pos] == '\0')
		&& !args->inside_quotes)
		token = save_word(token, cmdline, &args->last_pos, args->pos);
	if (!token)
		return (NULL);
	if (cmdline[args->pos] == '|')
		token = save_symbol(token, "|", &args->last_pos, &args->pos);
	else if (ft_contains("\"'", cmdline[args->pos]))
	{
		if (args->pos != 0 && ft_contains(" \f\n\r\t\v", cmdline[args->pos - 1])
			&& !args->inside_quotes)
			token = save_quotes(token, cmdline, &args->last_pos, &args->pos);
		else
			args->inside_quotes = !args->inside_quotes;
	}
	else if (ft_startswith(cmdline + args->pos, "<<"))
		token = save_symbol(token, "<<", &args->last_pos, &args->pos);
	else if (ft_startswith(cmdline + args->pos, ">>"))
		token = save_symbol(token, ">>", &args->last_pos, &args->pos);
	else if (cmdline[args->pos] == '>')
		token = save_symbol(token, ">", &args->last_pos, &args->pos);
	else if (cmdline[args->pos] == '<')
		token = save_symbol(token, "<", &args->last_pos, &args->pos);
	return (token);
}
