/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 13:35:25 by alermolo          #+#    #+#             */
/*   Updated: 2024/01/16 14:30:11 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static int	wait_for_children(t_pipe *pipex)
{
	int	i;
	int	status;

	i = 0;
	while (i < pipex->cmd_count)
		waitpid(pipex->pids[i++], &status, 0);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	return (status);
}

static void	set_fd(t_pipe *pipex, int cmd_no, int fd[4])
{
	if (cmd_no == 0)
	{
		fd[2] = pipex->in_fd;
		fd[3] = fd[1];
	}
	else if (cmd_no < pipex->cmd_count - 1)
		fd[3] = fd[1];
	else if (cmd_no == pipex->cmd_count - 1)
		fd[3] = pipex->out_fd;
}

static void	exec_child(t_pipe *pipex, int cmd_no, int fd[4], char **env)
{
	if (fd[2] == -1 || fd[3] == -1)
		free_and_exit(pipex, EXIT_FAILURE);
	close(fd[0]);
	if (dup2(fd[2], STDIN_FILENO) == -1)
	{
		perror(NULL);
		free_and_exit(pipex, EXIT_FAILURE);
	}
	close(fd[2]);
	if (dup2(fd[3], STDOUT_FILENO) == -1)
	{
		perror(NULL);
		free_and_exit(pipex, EXIT_FAILURE);
	}
	close(fd[3]);
	if (!(pipex->paths[cmd_no]))
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free_and_exit(pipex, 127);
	}
	execve(pipex->paths[cmd_no], pipex->cmds[cmd_no], env);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	free_and_exit(pipex, EXIT_FAILURE);
}

int	exec_cmd(t_pipe *pipex, char **env)
{
	int	cmd_no;
	int	status;

	cmd_no = 0;
	while (cmd_no < pipex->cmd_count)
	{
		if (pipe(pipex->fd) == -1)
			free_and_exit(pipex, EXIT_FAILURE);
		pipex->pids[cmd_no] = fork();
		if (pipex->pids[cmd_no] < 0)
			free_and_exit(pipex, EXIT_FAILURE);
		set_fd(pipex, cmd_no, pipex->fd);
		if (pipex->pids[cmd_no] == 0)
			exec_child(pipex, cmd_no, pipex->fd, env);
		if (pipex->fd[2] > 0)
			close(pipex->fd[2]);
		if (pipex->fd[3] > 0)
			close(pipex->fd[3]);
		pipex->fd[2] = pipex->fd[0];
		cmd_no++;
	}
	status = wait_for_children(pipex);
	return (status);
}
