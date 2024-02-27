/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 17:01:53 by alermolo          #+#    #+#             */
/*   Updated: 2024/01/15 17:43:16 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	free_struct(t_pipe *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count)
		free(pipex->paths[i++]);
	free(pipex->paths);
	i = 0;
	while (pipex->cmds[i])
		free_arr(pipex->cmds[i++]);
	free(pipex->cmds);
	free(pipex->pids);
	if (pipex->in_fd > 0)
		close(pipex->in_fd);
	if (pipex->out_fd > 0)
		close(pipex->out_fd);
	i = 0;
	while (i < 4)
	{
		if (pipex->fd[i] > 0)
			close(pipex->fd[i]);
		i++;
	}
	if (pipex->has_heredoc)
		unlink("here_doc");
}

void	free_and_exit(t_pipe *pipex, int exit_status)
{
	free_struct(pipex);
	exit(exit_status);
}
