/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 13:54:06 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/12 16:57:09 by alermolo         ###   ########.fr       */
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

static void	set_fd(t_pipe *pipex, int cmd_no)
{
	pipex->fd[3] = pipex->fd[1];
	if (cmd_no == pipex->cmd_count - 1)
		pipex->fd[3] = STDOUT_FILENO;
	if (cmd_no == 0)
		pipex->fd[2] = STDIN_FILENO;
}

static void	create_heredoc(t_pipe *pipex, t_redir *redir)
{
	char	*line;
	char	*limiter;

	pipex->fd[2] = open("here_doc", O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (pipex->fd[2] < 0)
		joint_error_msg("here_doc");
	line = get_next_line(0);
	limiter = ft_strjoin(redir->file, "\n");
	while (line && ft_strcmp(line, limiter) != 0)
	{
		write(pipex->fd[2], line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	free(line);
	free(limiter);
	close(pipex->fd[2]);
}

static void redirect(t_pipe *pipex, t_block *cmd_lst)
{
	while (cmd_lst->redir)
	{
		if (cmd_lst->redir->type == REDIRECT_IN)
		{
			if (pipex->fd[2] > 0)
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
		else if (cmd_lst->redir->type == REDIRECT_HEREDOC)
		{
			if (pipex->fd[2] > 0)
				close(pipex->fd[2]);
			create_heredoc(pipex, cmd_lst->redir);
			pipex->fd[2] = open("here_doc", O_RDONLY);
		}
		if (pipex->fd[2] == -1 || pipex->fd[2] == -1)
		{
			perror(NULL);
			free_and_exit(pipex, EXIT_FAILURE);
		}
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
		set_fd(pipex, cmd_no);
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
