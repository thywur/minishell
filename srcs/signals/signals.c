/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:14:56 by quteriss          #+#    #+#             */
/*   Updated: 2024/04/08 13:35:28 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler_main(int sig)
{
	(void)sig;
	g_last_signal = 130;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sig_handler_child(int sig)
{
	if (sig == SIGINT)
	{
		g_last_signal = 130;
		write(2, "\n", 1);
	}
	if (sig == SIGQUIT)
	{
		g_last_signal = 131;
		write(2, "Quit (core dumped)\n", 19);
	}
}

void	sig_handler_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		(void)sig;
		// dprintf(2, "tut\n");
		write(2, "\n", 1);
		close(STDIN_FILENO);
		g_last_signal = 130;
	}
	if (sig == SIGQUIT)
	{
		(void)sig;
		 write(2, "\b\b  \b\b", 6);
	}
}
