/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 13:35:25 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/05 12:07:35 by alermolo         ###   ########.fr       */
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

static void	set_fd(t_pipe *pipex, t_block *cmd_lst, int cmd_no, int fd[4])
{
	fd[3] = fd[1];
	if (cmd_no == pipex->cmd_count - 1)
		fd[3] = STDOUT_FILENO;
	if (cmd_no == 0)
		fd[2] = 0;
	if (cmd_lst->redir)
	{
		// cmd_lst->redir = ft_lstlast(cmd_lst->redir);
		if (cmd_lst->redir->type == REDIRECT_IN)
			pipex->fd[2] = open(cmd_lst->redir->file, O_RDONLY);
		else if (cmd_lst->redir->type == REDIRECT_OUT)
			pipex->fd[3] = open(cmd_lst->redir->file, O_CREAT | O_RDWR | O_TRUNC, 0644);
		else if (cmd_lst->redir->type == REDIRECT_APPEND)
			pipex->fd[3] = open(cmd_lst->redir->file, O_CREAT | O_RDWR | O_APPEND, 0644);
		// else if (cmd_lst->redir->type == REDIRECT_HEREDOC)
		// 	read_heredoc();
		if (pipex->fd[2] == -1 || pipex->fd[3] == -1)
		{
			perror(NULL);
			free_and_exit(pipex, EXIT_FAILURE);
		}
	}
	// else if (cmd_no == pipex->cmd_count - 1)
	// 	fd[3] = pipex->out_fd;
}

static void	redirect(t_pipe *pipex, t_block *cmd_lst, char **env, int cmd_no)
{
	pipex->fd[3] = pipex->fd[1];
	if (cmd_no == pipex->cmd_count - 1)
		pipex->fd[3] = STDOUT_FILENO;
	if (cmd_no == 0)
		pipex->fd[2] = 0;
	if (cmd_lst->redir->type == REDIRECT_IN)
		pipex->fd[2] = open(cmd_lst->redir->file, O_RDONLY);
	else if (cmd_lst->redir->type == REDIRECT_OUT)
		pipex->fd[3] = open(cmd_lst->redir->file, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else if (cmd_lst->redir->type == REDIRECT_APPEND)
		pipex->fd[3] = open(cmd_lst->redir->file, O_CREAT | O_RDWR | O_APPEND, 0644);
	// else if (cmd_lst->redir->type == REDIRECT_HEREDOC)
	// 	read_heredoc();
	if (pipex->fd[2] == -1 || pipex->fd[3] == -1)
	{
		perror(NULL);
		free_and_exit(pipex, EXIT_FAILURE);
	}
	// pipex->fd[3] = pipex->fd[1];
	printf("executing redirection\n");
	if (pipex->pids[cmd_no] == 0)
		exec_child(pipex, cmd_lst, cmd_no, env);
	if (pipex->fd[2] > 0)
		close(pipex->fd[2]);
	if (pipex->fd[3] > 0)
		close(pipex->fd[3]);
}

int	exec_cmd(t_pipe *pipex, t_block *cmd_lst, char **env)
{
	int	cmd_no;
	int	status;

	cmd_no = 0;
	while (cmd_no < pipex->cmd_count && cmd_lst)
	{
		if (pipe(pipex->fd) == -1)
			free_and_exit(pipex, EXIT_FAILURE);
		pipex->pids[cmd_no] = fork();
		if (pipex->pids[cmd_no] < 0)
			free_and_exit(pipex, EXIT_FAILURE);
		if (cmd_lst->redir)
		{
			while (cmd_lst->redir->next)
			{
				// set_fd(pipex, cmd_lst, cmd_no, pipex->fd);
				redirect(pipex, cmd_lst, env, cmd_no);
				cmd_lst->redir = cmd_lst->redir->next;
			}
		}
		set_fd(pipex, cmd_lst, cmd_no, pipex->fd);
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
	status = wait_for_children(pipex);
	return (status);
}
/*
int	exec_cmd(t_block *cmd_lst, char **env)
{
	int	cmd_no;
	t_block	*tmp;
	int	status;
	int	fd[4];

	cmd_no = 0;
	tmp = cmd_lst;
	while (tmp)
	{
		if (pipe(fd) == -1)
			return (-1);
		tmp->pid = fork();
		if (tmp->pid < 0)
			return (-1);
		if (cmd_lst->redir)
		{
			while (cmd_lst->redir->next)
				redirect(pipex, cmd_lst, env, cmd_no);
		}
		set_fd(pipex, cmd_lst, cmd_no, pipex->fd);
		if (tmp->pid == 0)
			exec_child(cmd_lst, cmd_no, env);
		if (fd[2] > 0)
			close(fd[2]);
		if (fd[3] > 0)
			close(fd[3]);
		fd[2] = fd[0];
		tmp = tmp->next;
	}
	// status = wait_for_children(pipex);
	return (status);
}
 */
