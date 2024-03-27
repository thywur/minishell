/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 15:10:34 by quentinteri       #+#    #+#             */
/*   Updated: 2024/03/27 13:18:57 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dup_env(char ***env)
{
	char **new_env;
	int i;

	new_env = malloc(sizeof(char *) * (ft_arrlen(*env) + 1));
	if (!new_env)
		return (print_error(MALLOC_ERROR), 1);
	i = 0;
	while ((*env)[i])
	{
		new_env[i] = ft_strdup((*env)[i]);
		if (!new_env[i])
			return (free_arr(new_env), print_error(MALLOC_ERROR), -1);
		i++;
	}
	new_env[i] = NULL;
	*env = new_env;
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char	*cmdline;
	t_token	*tokens;
	t_block	*blocks;
	int		saved_stdin;
	int		saved_stdout;

	(void)argv;
	if (argc > 1)
		return (1);
	if (dup_env(&env) == -1)
		return (1);
	while (42)
	{
		cmdline = readline(">>> ");
		if (!ft_strcmpr(cmdline, "quit"))
			return (0);
		add_history(cmdline);
		tokens = split_cmdline_into_tokens(cmdline);
		expand_tokens(&tokens, env);
		blocks = join_tokens_into_blocks(&tokens);
		if (!blocks)
			return (print_error(MALLOC_ERROR), 1);
		// print_blocks(&blocks);
		saved_stdin = dup(STDIN_FILENO);		// trouver un autre moyen de les stocker et de les close
		saved_stdout = dup(STDOUT_FILENO);
		cmd_handler(&blocks, &env);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		free_blocks(&blocks);
	}
}
