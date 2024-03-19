/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quentinterisse <quentinterisse@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 15:10:34 by quentinteri       #+#    #+#             */
/*   Updated: 2024/03/18 15:49:51 by quentinteri      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*cmdline;
	t_token	*tokens;
	t_block	*blocks;

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
		// print_tokens(&tokens);
		// free_tokens(&tokens);
		blocks = join_tokens_into_blocks(&tokens);
		print_blocks(&blocks);
	}
}
