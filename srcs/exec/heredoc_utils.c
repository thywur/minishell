/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:21:54 by alermolo          #+#    #+#             */
/*   Updated: 2024/04/04 16:00:34 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_string(char *str, char **env)
{
	char	*new_str;
	int		size;

	size = get_final_data_size(str, env, 0, g_last_signal);
	if (size == -1)
		return (free(str), NULL);
	new_str = expand_token(str, env, size, g_last_signal);
	return (new_str);
}

char	*readline_heredoc(char **env)
{
	char	*line;

	line = readline("> ");
	if (line)
		line = ft_strjoin_free(line, "\n");
	if (line && ft_contains(line, '$'))
		line = expand_string(line, env);
	return (line);
}

void	err_putchar(char c)
{
	write(STDERR_FILENO, &c, 1);
}

void	err_putnbr(unsigned int nb)
{
	if (nb > 9)
	{
		err_putnbr(nb / 10);
		err_putnbr(nb % 10);
	}
	else
		err_putchar(nb + 48);
}

void	err_heredoc(char *limiter, int line_no)
{
	size_t	i;

	write(2, "minishell : warning: here-document at line ", 43);
	err_putnbr(line_no);
	write(2, " delimited by end-of-file (wanted '", 35);
	i = 0;
	while (i < ft_strlen(limiter) - 1)
		err_putchar(limiter[i++]);
	write(2, "')\n", 3);
	g_last_signal = 130;
}
