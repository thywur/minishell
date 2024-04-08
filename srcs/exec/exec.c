/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 13:54:06 by alermolo          #+#    #+#             */
/*   Updated: 2024/04/08 12:57:36 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_for_children(t_pipe *pipex)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < pipex->cmd_count)
	{
		waitpid(pipex->pids[i], &status, 0);
		i++;
	}
	if (WIFEXITED(status) && !pipex->has_heredoc)
		g_last_signal = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		sig_handler_child(WTERMSIG(status));
	return (g_last_signal);
}

static void	path_not_found(t_pipe *pipex, t_block *cmd_lst, char **env)
{
	if (is_builtin(cmd_lst->cmd))
	{
		exec_builtin(pipex->fd, cmd_lst, pipex, &env);
		free_and_exit(pipex, cmd_lst, env, EXIT_SUCCESS);
	}
	else
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free_and_exit(pipex, cmd_lst, env, 127);
	}
}

static void	exec_child(t_pipe *pipex, t_block *cmd_lst, int cmd_no, char ***env)
{
	if (pipex->fd[2] == -1 || pipex->fd[3] == -1)
		free_and_exit(pipex, cmd_lst, *env, EXIT_FAILURE);
	if (pipex->fd[0] != 0)
		close(pipex->fd[0]);
	if (dup2(pipex->fd[2], STDIN_FILENO) == -1)
	{
		perror(NULL);
		free_and_exit(pipex, cmd_lst, *env, EXIT_FAILURE);
	}
	if (pipex->fd[2] != 0)
		close(pipex->fd[2]);
	if (dup2(pipex->fd[3], STDOUT_FILENO) == -1)
	{
		perror(NULL);
		free_and_exit(pipex, cmd_lst, *env, EXIT_FAILURE);
	}
	if (pipex->fd[3] != 1)
		close(pipex->fd[3]);
	if (!(pipex->paths[cmd_no]))
		path_not_found(pipex, cmd_lst, *env);
	execve(pipex->paths[cmd_no], cmd_lst->args, *env);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	free_and_exit(pipex, cmd_lst, *env, EXIT_FAILURE);
}

static void	set_fd(t_pipe *pipex, int cmd_no)
{
	pipex->fd[3] = pipex->fd[1];
	if (cmd_no == pipex->cmd_count - 1)
		pipex->fd[3] = STDOUT_FILENO;
	if (cmd_no == 0)
		pipex->fd[2] = STDIN_FILENO;
}

int	exec_cmd(t_pipe *pipex, t_block *cmd_lst, char ***env)
{
	int	cmd_no;

	cmd_no = 0;
	while (cmd_no < pipex->cmd_count && cmd_lst)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		if (pipe(pipex->fd) == -1)
			free_and_exit(pipex, cmd_lst, *env, EXIT_FAILURE);
		set_fd(pipex, cmd_no);
		if (cmd_lst->redir)
			redirect(pipex, cmd_lst, env);
		pipex->pids[cmd_no] = fork();
		if (pipex->pids[cmd_no] < 0)
			free_and_exit(pipex, cmd_lst, *env, EXIT_FAILURE);
		if (pipex->pids[cmd_no] == 0)
		{
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			signal(SIGINT, &sig_handler_child);
			signal(SIGQUIT, &sig_handler_child);
			exec_child(pipex, cmd_lst, cmd_no, env);
		}
		if (pipex->fd[2] > 0)
			close(pipex->fd[2]);
		if (pipex->fd[3] > 0)
			close(pipex->fd[3]);
		pipex->fd[2] = pipex->fd[0];
		cmd_lst = cmd_lst->next;
		cmd_no++;
	}
	return (wait_for_children(pipex));
}
