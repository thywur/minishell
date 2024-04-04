/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:41:45 by quteriss          #+#    #+#             */
/*   Updated: 2024/04/04 14:28:09 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	store_field(char **env, char *word, char **field)
{
	int		k;

	k = 0;
	while (env[k] && !ft_startswith(env[k], word))
		k++;
	if (env[k])
		*field = env[k] + ft_strlen(word);
	else
		*field = env[0] + ft_strlen(env[0]);
}

char	*expand_variable(char *data, char **env, int *i, int exit_status)
{
	char	*field;
	char	*word;
	int		j;

	j = 0;
	while (data[j] && !ft_contains(" \t\n\'\"", data[j])
		&& !(j != 0 && data[j] == '$'))
		j++;
	field = ft_substr(data, 1, j - 1);
	if (!field)
		return (NULL);
	if (!ft_strcmpr(field, "?"))
	{
		*i += 1;
		return (ft_itoa(exit_status));
	}
	word = ft_strjoin(field, "=");
	free(field);
	if (!word)
		return (NULL);
	store_field(env, word, &field);
	free(word);
	*i += j - 1;
	return (ft_strdup(field));
}

int	get_final_data_size(char *data, char **env, int size, int exit_status)
{
	char	*field;
	char	quote;
	int		i;
	int		k;

	i = 0;
	quote = 0;
	while (data[i])
	{
		if (data[i] == '$' && quote != '\'')
		{
			field = expand_variable(data + i, env, &i, exit_status);
			k = ft_secured_strlen(field);
			free(field);
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

char	*expand_token(char *data, char **env, int size, int exit_status)
{
	char	*field;
	char	quote;
	char	*word;
	int		j;
	int		i;

	i = -1;
	j = 0;
	quote = 0;
	word = malloc(sizeof(char) * (size + 1));
	if (!word)
		return (NULL);
	while (data[++i])
	{
		if (data[i] == '$' && quote != '\'')
		{
			field = expand_variable(data + i, env, &i, exit_status);
			j += ft_strcpy(word + j, field);
			free(field);
		}
		else if (!quote && ft_contains("\"'", data[i]))
			quote = data[i];
		else if (quote && quote == data[i])
			quote = 0;
		else
			word[j++] = data[i];
	}
	word[size] = '\0';
	free(data);
	return (word);
}

int	expand_tokens(t_token **tokens, char **env, int exit_status)
{
	t_token	*token;
	int		size;

	token = *tokens;
	while (token && token->next)
	{
		trim_token_data(token);
		if (!token->data)
			return (1);
		size = get_final_data_size(token->data, env, 0, exit_status);
		if (size == -1)
			return (1);
		token->data = expand_token(token->data, env, size, exit_status);
		if (!token->data)
			return (1);
		token = token->next;
	}
	return (0);
}
