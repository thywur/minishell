/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:05:07 by alermolo          #+#    #+#             */
/*   Updated: 2024/04/08 14:40:44 by alermolo         ###   ########.fr       */
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
	pipex->has_heredoc = 0;
	pipex->saved_stdin = dup(STDIN_FILENO);
	// // dprintf(2, "saved stdin fd %d\n", pipex->saved_stdin);
	if (pipex->saved_stdin == -1)
		perror(NULL);
	pipex->saved_stdout = dup(STDOUT_FILENO);
	// // dprintf(2, "saved stdout fd %d\n", pipex->saved_stdout);
	if (pipex->saved_stdout == -1)
		perror(NULL);
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
