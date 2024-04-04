/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:21:54 by alermolo          #+#    #+#             */
/*   Updated: 2024/04/04 14:29:11 by alermolo         ###   ########.fr       */
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
