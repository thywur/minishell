/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:14:56 by quteriss          #+#    #+#             */
/*   Updated: 2024/03/30 15:57:54 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int signal)
{
	if (signal == SIGINT)
	{
		write(2, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	// last_signal = signal;
}

void	signal_handler(int signal)
{
	sigint_handler(signal);
	rl_redisplay();
}

void	signal_exec_handler(int signal)
{
	if (signal == SIGINT)
		sigint_handler(signal);
	if (signal == SIGQUIT)
	{
		// g_status_code = 131;
		write(2, "Quit (core dumped)\n", 19);
	}
}

void	signal_hub(char mod)
{
	struct sigaction	act;

	act.sa_flags = 0;
	if (mod == 1)
	{
		act.sa_handler = &signal_handler;
		sigemptyset(&act.sa_mask);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mod == 2)
	{
		act.sa_handler = &signal_exec_handler;
		sigemptyset(&act.sa_mask);
		act.sa_flags = SA_SIGINFO;
		sigaction(SIGQUIT, &act, NULL);
	}
	else if (mod == 3)
	{
		act.sa_handler = &sigint_handler;
		sigemptyset(&act.sa_mask);
		signal(SIGQUIT, SIG_IGN);
	}
	sigaction(SIGINT, &act, NULL);
}
