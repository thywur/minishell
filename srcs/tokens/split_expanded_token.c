/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_expanded_token.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 18:33:41 by quteriss          #+#    #+#             */
/*   Updated: 2024/04/09 12:54:20 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_words_with_quotes(char *str)
{
	int	count;
	int	in_quotes;

	count = 0;
	in_quotes = 0;
	while (*str)
	{
		while (*str == ' ')
			str++;
		if (*str != '\0')
		{
			while (*str != '\0' && (*str != ' ' || in_quotes))
			{
				if (!in_quotes && (*str == '"' || *str == '\''))
					in_quotes = *str;
				else if (in_quotes == *str)
					in_quotes = 0;
				str++;
			}
			count++;
		}
	}
	return (count);
}

char	*split_word(char *str, int *pos)
{
	char	in_quotes;
	char	*word;
	int		size;
	int		i;

	size = 0;
	in_quotes = 0;
	while (str[size] && (str[size] != ' ' || in_quotes))
	{
		if (!in_quotes && (str[size] == '"' || str[size] == '\''))
			in_quotes = str[size];
		else if (in_quotes == str[size])
			in_quotes = 0;
		size++;
	}
	word = malloc(sizeof(char) * (size + 1));
	if (!word)
		return (NULL);
	i = -1;
	while (++i < size)
		word[i] = str[i];
	word[i] = '\0';
	while (str[size] && ft_contains(" \t\n", str[size]))
		size ++;
	*pos = *pos + size;
	return (word);
}

char	**split_with_quotes(char *str)
{
	char 	**arr;
	int		count;
	int		pos;
	int		i;

	i = 0;
	pos = 0;
	while (str[pos] && ft_contains(" \t\n", str[pos]))
		pos++;
	count = count_words_with_quotes(str + pos);
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	while (i < count)
	{
		arr[i] = split_word(str + pos, &pos);
		if (!arr[i])
			return (free_arr(arr), NULL);
		i++;
	}
	arr[count] = NULL;
	return (arr);
}

t_token	*new_word_token(char *data, t_token	*next_elem)
{
	t_token	*new_token;

	new_token = create_empty_token();
	if (!new_token)
		return (NULL);
	new_token->type = WORD;
	new_token->is_inquote = is_inquote(data);
	new_token->data = ft_strdup(data);
	if (!new_token->data)
		return (free_tokens(&new_token), NULL);
	new_token->next = next_elem;
	return (new_token);
}

int	split_expanded_token(t_token **tokens, int i)
{
	t_token	*new_token;
	char	**arr;
	t_token	*elem;

	arr = split_with_quotes((*tokens)->data);
	if (arr && ft_arrlen(arr) > 1)
	{
		elem = *tokens;
		if (elem->data)
			free(elem->data);
		elem->data = ft_strdup(arr[i++]);
		if (!elem->data)
			return (free_arr(arr), 1);
		while (arr[i])
		{
			new_token = new_word_token(arr[i], elem->next);
			if (!new_token)
				return (free_arr(arr), 1);
			elem->next = new_token;
			elem = new_token;
			i++;
		}
		*tokens = new_token;
	}
	return (free_arr(arr), 0);
}
