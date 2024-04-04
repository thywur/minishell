/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 14:14:56 by quteriss          #+#    #+#             */
/*   Updated: 2024/04/04 16:04:44 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void check_signal_handler(int sig)
{
    struct sigaction old_action;

    if (sigaction(sig, NULL, &old_action) < 0) {
        perror("sigaction");
        return;
    }

    if (old_action.sa_handler == SIG_DFL) {
        dprintf(2, "Default handler for signal %d\n", sig);
    } else if (old_action.sa_handler == SIG_IGN) {
        dprintf(2, "Ignore handler for signal %d\n", sig);
    } else {
        dprintf(2, "Custom handler for signal %d at address %p\n", sig, old_action.sa_handler);
    }
}

void	handle_sigchild(int signal)
{
	(void)signal;
	// dprintf(2, "sigchild handler, last signal = %d\n", g_last_signal);
	if (g_last_signal == 2)
	{
		g_last_signal = 0;
		write(2, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		// rl_redisplay();
	}
	if (g_last_signal == 131)
		write(2, "Quit (core dumped)\n", 19);
}

void	sigint_handler(int sig)
{
	// dprintf(2, "sigint handler, sig = %d\n", sig);
	g_last_signal = sig;
	if (sig == SIGINT)
	{
		g_last_signal = 130;
		write(2, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	else if (sig == SIGCHLD)
	{
		g_last_signal = 0;
		write(2, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	// rl_redisplay();
}

void	signal_handler(int signal)
{
	// dprintf(2, "here, signal = %d\n", signal);
	sigint_handler(signal);
	if (signal == SIGINT)
	{
		g_last_signal = 130;
		rl_redisplay();
	}
	if (signal == SIGQUIT)
	{
		g_last_signal = 131;
		write(2, "Quit (core dumped)\n", 19);
	}
}

void	signal_exec_handler(int signal)
{
	dprintf(2, "exec signal %d\n", signal);
	if (signal == SIGINT)
	{
		dprintf(2, "exec sigint\n");
		sigint_handler(signal);
		g_last_signal = 130;
	}
}

// void	signal_handler_test(int sig)
// {

// }

void	signal_hub(int mod)
{
	struct sigaction	act;

	// act.sa_flags = 0;
	// ft_bzero(&act, sizeof(act));
	if (mod == 1)
	{
		// printf("sig main\n");
		act.sa_handler = signal_handler;
		sigemptyset(&act.sa_mask);
		// check_signal_handler(SIGQUIT);
		sigaction(SIGINT, &act, NULL);
		// sigaction(SIGQUIT, &act, NULL);
		signal(SIGQUIT, SIG_IGN);
		// check_signal_handler(SIGQUIT);
	}
	else if (mod == 2)
	{
		// printf("sig child\n");
		// ft_bzero(&act, sizeof(act));
		sigemptyset(&act.sa_mask);
		act.sa_handler = signal_exec_handler;
		act.sa_flags = SA_SIGINFO;
		// check_signal_handler(SIGQUIT);
		sigaction(SIGINT, &act, NULL);
		sigaction(SIGQUIT, &act, NULL);
		// check_signal_handler(SIGQUIT);
	}
	else if (mod == 3)			//sert a rien ?
	{
		// printf("sig heredoc\n");
		act.sa_handler = &signal_handler;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGINT, &act, NULL);
		signal(SIGQUIT, SIG_IGN);
	}
	sigaction(SIGINT, &act, NULL);
}


//ctrl-d in the middle of sentence flushes stdin

