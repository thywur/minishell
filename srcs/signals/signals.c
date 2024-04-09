/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:14:56 by quteriss          #+#    #+#             */
/*   Updated: 2024/04/09 16:16:57 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler_main(int sig)
{
	(void)sig;
	g_status = 130;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sig_handler_child(int sig)
{
	if (sig == SIGINT)
	{
		g_status = 130;
		write(2, "\n", 1);
	}
	if (sig == SIGQUIT)
	{
		g_status = 131;
		write(2, "Quit (core dumped)\n", 19);
	}
}

void	sig_handler_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		(void)sig;
		close(STDIN_FILENO);
		g_status = 130;
	}
	if (sig == SIGQUIT)
	{
		(void)sig;
		write(2, "\b\b  \b\b", 6);
	}
}

void	launch_heredoc_sig_catcher(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sig_handler_heredoc);
	signal(SIGQUIT, &sig_handler_heredoc);
}

void	launch_child_sig_catcher(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &sig_handler_child);
	signal(SIGQUIT, &sig_handler_child);
}
