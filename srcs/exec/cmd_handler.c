/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:05:07 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/30 16:08:39 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_pipex(t_pipe *pipex, t_block *cmd_lst, char ***env)
{
	pipex->paths = NULL;
	pipex->cmd_count = ft_lstsize(cmd_lst);
	pipex->pids = malloc(sizeof(int) * pipex->cmd_count);
	if (!pipex->pids)
		free_and_exit(pipex, cmd_lst, *env, EXIT_FAILURE);
	pipex->fd[0] = -1;
	pipex->fd[1] = -1;
	pipex->fd[2] = -1;
	pipex->fd[3] = -1;
}

int	cmd_handler(t_block **blocks, char ***env)
{
	t_pipe	pipex;
	int		status;
	t_block	*cmd_lst;

	if (!blocks)
		return (EXIT_FAILURE);
	cmd_lst = *blocks;
	init_pipex(&pipex, cmd_lst, env);
	if (is_builtin(cmd_lst->cmd) && !cmd_lst->next)
	{
		status = exec_builtin(pipex.fd, cmd_lst, &pipex, env);
		free_pipex(&pipex);
		return (status);
	}
	combine_paths(env, &pipex, cmd_lst);
	status = exec_cmd(&pipex, cmd_lst, env);
	free_pipex(&pipex);
	return (status);
}
