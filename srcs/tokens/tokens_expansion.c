/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quentinterisse <quentinterisse@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:41:45 by quteriss          #+#    #+#             */
/*   Updated: 2024/03/18 13:30:52 by quentinteri      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_variable(char *data, char **env, int *i)
{
	char	*_word;
	char	*word;
	int		j;
	int		k;

	j = 0;
	while (data[j] && !ft_contains(" \t\'\"", data[j]))
		j++;
	_word = ft_substr(data, 1, j - 1);
	if (!_word)
		return (NULL);
	word = ft_strjoin(_word, "=");
	free(_word);
	if (!word)
		return (NULL);
	k = 0;
	while (env[k] && !ft_startswith(env[k], word))
		k++;
	*i += j - 1;
	if (env[k])
		_word = env[k] + ft_strlen(word);
	else
		_word = env[0] + ft_strlen(env[0]);
	free(word);
	return (_word);
}

int	get_final_data_size(char *data, char **env, int size)
{
	char	quote;
	int		i;
	int		k;

	i = 0;
	quote = 0;
	while (data[i])
	{
		if (data[i] == '$' && quote != '\'')
		{
			k = ft_secured_strlen(expand_variable(data + i, env, &i));
			if (k == -1)
				return (k);
			size += k;
		}
		else if (!quote && ft_contains("\"'", data[i]))
			quote = data[i];
		else if (quote && quote == data[i])
			quote = 0;
		else
			size++;
		i++;
	}
	return (size);
}

char	*expand_token(char *data, char **env, int size)
{
	char	quote;
	char	*word;
	int		i;
	int		j;

	i = 0;
	j = 0;
	quote = 0;
	word = malloc(sizeof(char) * (size + 1));
	if (!word)
		return (NULL);
	while (data[i])
	{
		if (data[i] == '$' && quote != '\'')
			j += ft_strcpy(word + j, expand_variable(data + i, env, &i));
		else if (!quote && ft_contains("\"'", data[i]))
			quote = data[i];
		else if (quote && quote == data[i])
			quote = 0;
		else
			word[j++] = data[i];
		i++;
	}
	word[size] = '\0';
	return (word);
}

int	format_tokens_data(t_token **tokens, char **env)
{
	t_token	*prev_token;
	t_token	*token;
	int		size;

	token = *tokens;
	prev_token = NULL;
	while (token->next)
	{
		if (token->type != WORD && token->type != 0
			&& (!prev_token || prev_token->type != WORD || !token->next))
			return (1);
		trim_token_data(token);
		if (!token->data)
			return (2);
		size = get_final_data_size(token->data, env, 0);
		if (size == -1)
			return (2);
		token->data = expand_token(token->data, env, size);
		if (!token->data)
			return (2);
		prev_token = token;
		token = token->next;
	}
	return (0);
}

t_token	**expand_tokens(t_token **tokens, char **env)
{
	int	err_status;

	err_status = format_tokens_data(tokens, env);
	if (err_status == 1)
		return (free_tokens(tokens),
			print_error("syntax error near unexpected token"), NULL);
	else if (err_status == 2)
		return (free_tokens(tokens),
			print_error(MALLOC_ERROR), NULL);
	return (tokens);
}
