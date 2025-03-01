/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_expansion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 10:41:45 by quteriss          #+#    #+#             */
/*   Updated: 2024/04/09 14:43:22 by quteriss         ###   ########.fr       */
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
		free(field);
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

int	get_final_data_size(char *data, char **env, int size, int check_quotes)
{
	char	*field;
	char	quote;
	int		i;
	int		k;

	i = 0;
	quote = 0;
	while (data[i])
	{
		if (data[i] == '$' && !(quote == '\'' && check_quotes))
		{
			field = expand_variable(data + i, env, &i, g_status);
			k = ft_secured_strlen(field);
			free(field);
			if (k == -1)
				return (k);
			size += k;
		}
		else
			get_final_data_size_extra(&quote, data[i], &size);
		i++;
	}
	return (size);
}

char	*expand_token(char *data, char **env, int size, int check_quotes)
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
		if (data[i] == '$' && !(quote == '\'' && check_quotes))
		{
			field = expand_variable(data + i, env, &i, g_status);
			j += ft_strcpy(word + j, field);
			free(field);
		}
		else
			expand_token_extra(&quote, data[i], word, &j);
	}
	word[size] = '\0';
	return (free(data), word);
}

int	expand_tokens(t_token **tokens, char **env)
{
	t_token	*prev_token;
	t_token	*token;
	int		size;

	token = *tokens;
	prev_token = NULL;
	while (token && token->next)
	{
		trim_token_data(token);
		if (!token->data)
			return (1);
		token->is_inquote = is_inquote(token->data);
		size = get_final_data_size(token->data, env, 0, 1);
		if (size == -1)
			return (1);
		if ((prev_token && prev_token->type != REDIRECT_HEREDOC) || !prev_token)
			token->data = expand_token(token->data, env, size, 1);
		if (!token->data)
			return (1);
		if (!token->is_inquote && split_expanded_token(&token, 0))
			return (1);
		prev_token = token;
		token = token->next;
	}
	return (0);
}
