/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 13:54:06 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/06 16:38:33 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	exec_child(t_pipe *pipex, t_block *cmd_lst, int cmd_no, char **env)
{
	// printf("all fds in: %d out: %d, pipe %d %d\n", pipex->fd[0], pipex->fd[1], pipex->fd[2], pipex->fd[3]);
	printf("executing child cmd: %s fd in: %d fd out: %d\n", cmd_lst->cmd, pipex->fd[2], pipex->fd[3]);
	if (pipex->fd[2] == -1 || pipex->fd[3] == -1)
		free_and_exit(pipex, EXIT_FAILURE);
	if (pipex->fd[0] != 0)
		close(pipex->fd[0]);
	if (dup2(pipex->fd[2], STDIN_FILENO) == -1)
	{
		perror(NULL);
		free_and_exit(pipex, EXIT_FAILURE);
	}
	if (pipex->fd[2] != 0)
		close(pipex->fd[2]);
	if (dup2(pipex->fd[3], STDOUT_FILENO) == -1)
	{
		perror(NULL);
		free_and_exit(pipex, EXIT_FAILURE);
	}
	if (pipex->fd[3] != 1)
		close(pipex->fd[3]);
	if (!(pipex->paths[cmd_no]))
	{
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free_and_exit(pipex, 127);
	}
	execve(pipex->paths[cmd_no], cmd_lst->args, env);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	free_and_exit(pipex, EXIT_FAILURE);
}

static void	set_fd(t_pipe *pipex, t_block *cmd_lst, int cmd_no)
{
	printf("setting fds. in: %d out: %d\n", pipex->fd[2], pipex->fd[3]);
	// if (!cmd_lst->redir || (cmd_lst->redir && cmd_lst->redir->type != REDIRECT_OUT))
	// {
	// close(pipex->fd[1]);
	pipex->fd[3] = pipex->fd[1];
	if (cmd_no == pipex->cmd_count - 1)
		pipex->fd[3] = STDOUT_FILENO;
	// }
	if (cmd_no == 0) // && (!cmd_lst->redir || (cmd_lst->redir && cmd_lst->redir->type != REDIRECT_IN)))
		pipex->fd[2] = STDIN_FILENO;
	printf("fds set. in: %d out: %d\n", pipex->fd[2], pipex->fd[3]);
}

static void redirect(t_pipe *pipex, t_block *cmd_lst)
{
	while (cmd_lst->redir)
	{
		printf("pre redir fd in: %d fd out: %d\n", pipex->fd[2], pipex->fd[3]);
		if (cmd_lst->redir->type == REDIRECT_IN)
		{
			close(pipex->fd[2]);
			pipex->fd[2] = open(cmd_lst->redir->file, O_RDONLY);
		}
		else if (cmd_lst->redir->type == REDIRECT_OUT)
		{
			close(pipex->fd[3]);
			pipex->fd[3] = open(cmd_lst->redir->file, O_CREAT | O_RDWR | O_TRUNC, 0644);
		}
		else if (cmd_lst->redir->type == REDIRECT_APPEND)
		{
			close(pipex->fd[3]);
			pipex->fd[3] = open(cmd_lst->redir->file, O_CREAT | O_RDWR | O_APPEND, 0644);
		}
		// else if (cmd_lst->redir->type == REDIRECT_HEREDOC)
		// 	read_heredoc();
		if (pipex->fd[2] == -1 || pipex->fd[2] == -1)
		{
			perror(NULL);
			free_and_exit(pipex, EXIT_FAILURE);
		}
		printf("redir filename: %s\n", cmd_lst->redir->file);
		printf("post redir fd in: %d fd out: %d\n", pipex->fd[2], pipex->fd[3]);
		cmd_lst->redir = cmd_lst->redir->next;
	}
}

int	exec_cmd(t_pipe *pipex, t_block *cmd_lst, char **env)
{
	int cmd_no;

	cmd_no = 0;
	while (cmd_no < pipex->cmd_count && cmd_lst)
	{
		if (pipe(pipex->fd) == -1)
			free_and_exit(pipex, EXIT_FAILURE);
		printf("pipe created. in: %d out: %d\n", pipex->fd[0], pipex->fd[1]);

		set_fd(pipex, cmd_lst, cmd_no);
		if (cmd_lst->redir)
			redirect(pipex, cmd_lst);
		pipex->pids[cmd_no] = fork();
		if (pipex->pids[cmd_no] < 0)
			free_and_exit(pipex, EXIT_FAILURE);

		if (pipex->pids[cmd_no] == 0)
			exec_child(pipex, cmd_lst, cmd_no, env);
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
