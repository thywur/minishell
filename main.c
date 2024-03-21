/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 15:10:34 by quentinteri       #+#    #+#             */
/*   Updated: 2024/03/21 17:49:04 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		print_blocks(&blocks);
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		cmd_handler(&blocks, env);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		free_blocks(&blocks);
	}
}
