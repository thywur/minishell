/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 15:10:34 by quteriss          #+#    #+#             */
/*   Updated: 2024/04/08 17:09:35 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

int	dup_env(char ***env)
{
	char	**new_env;
	int		i;

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

char	*read_cmdline(t_block **blocks, char **env)
{
	char	*cmdline;

	// signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sig_handler_main);
	cmdline = readline(">>> ");
	if (!cmdline)
	{
		printf("exit\n");
		if (*blocks)
			free_blocks(blocks);
		free_arr(env);
		return (NULL);
	}
	return (cmdline);
}

t_block	*process_cmdline(char *cmdline, int *exit_status, char **env)
{
	t_token	*tokens;
	t_block	*blocks;

	tokens = split_cmdline_into_tokens(cmdline);
	if (!tokens)
		return (NULL);
	*exit_status = expand_tokens(&tokens, env, *exit_status);
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

void	execute_cmdline(t_block **blocks, char ***env, int *exit_status)
{
	*exit_status = cmd_handler(blocks, env);
	free_blocks(blocks);
}

int	main(int argc, char **argv, char **env)
{
	char	*cmdline;
	t_block	*blocks;

	(void)argv;
	if (argc > 1 || dup_env(&env) == -1)
		return (1);
	g_status = 0;
	blocks = NULL;
	while (42)
	{
		cmdline = read_cmdline(&blocks, env);
		if (!cmdline)
			return (g_status);
		if (ft_strlen(cmdline) == 0)
		{
			free(cmdline);
			continue ;
		}
		add_history(cmdline);
		blocks = process_cmdline(cmdline, &g_status, env);
		if (!blocks)
		{
			free(cmdline);
			continue ;
		}
		execute_cmdline(&blocks, &env, &g_status);
		free(cmdline);
	}
	return (g_status);
}
