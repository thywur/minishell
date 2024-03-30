/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 17:01:53 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/30 15:58:05 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		if (arr[i])
			free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_pipex(t_pipe *pipex)
{
	int	i;

	if (pipex->paths)
	{
		i = 0;
		while (i < pipex->cmd_count)
		{
			if (pipex->paths[i])
				free(pipex->paths[i]);
			i++;
		}
		free(pipex->paths);
	}
	free(pipex->pids);
	i = 0;
	while (i < 4)
	{
		if (pipex->fd[i] > 0)
			close(pipex->fd[i]);
		i++;
	}
	unlink("here_doc");
}

void	free_and_exit(t_pipe *pipex, t_block *blocks, char **env, int exit_status)
{
	free_pipex(pipex);
	free_blocks(&blocks);
	free_arr(env);
	exit(exit_status);
}
