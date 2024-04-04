/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 15:10:34 by quteriss          #+#    #+#             */
/*   Updated: 2024/04/04 15:01:19 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_last_signal;

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

t_block	*process_cmdline(char *cmdline, int *exit_status, char **env)
{
	t_token	*tokens;
	t_block	*blocks;

	tokens = split_cmdline_into_tokens(cmdline);
	if (!tokens)
		return (NULL);
	*exit_status = expand_tokens(&tokens, env, exit_status);
	if (*exit_status == 4)
		return (free_tokens(&tokens), print_error(MALLOC_ERROR), NULL);
	if (!check_tokens(&tokens))
		return (free_tokens(&tokens), NULL);
	blocks = join_tokens_into_blocks(&tokens);
	if (!blocks)
	{
		*exit_status = 4;
		return (print_error(MALLOC_ERROR), NULL);
	}
	return (blocks);
}

int	main(int argc, char **argv, char **env)
{
	char	*cmdline;
	t_token	*tokens;
	t_block	*blocks;
	int		saved_stdin;
	int		saved_stdout;
	int		exit_status;

	(void)argv;
	if (argc > 1)
		return (1);
	if (dup_env(&env) == -1)
		return (1);
	g_last_signal = 0;
	exit_status = 0;
	while (42)
	{
		signal_hub(1);
		cmdline = readline(">>> ");
		if (!cmdline)
			return (1);
		if (ft_strlen(cmdline) == 0)	
			continue ;
		add_history(cmdline);
		blocks = process_cmdline(cmdline, &exit_status, env);
		if (!blocks)
			continue ;
		saved_stdin = dup(STDIN_FILENO);		// trouver un autre moyen de les stocker et de les close
		saved_stdout = dup(STDOUT_FILENO);
		exit_status = cmd_handler(&blocks, &env);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		free_blocks(&blocks);
	}
}
