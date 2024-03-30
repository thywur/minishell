/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:05:07 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/29 16:19:39 by alermolo         ###   ########.fr       */
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
	// int		std_fd[4];

	if (!blocks)
		return (EXIT_FAILURE);
	cmd_lst = *blocks;
	// std_fd[2] = STDIN_FILENO;
	// std_fd[3] = STDOUT_FILENO;
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

/*
int	main(int argc, char **argv, char **env)
{
	t_block	block1;
	t_block block2;
	t_redir	redir1;
	t_redir redir1_1;
	t_redir	redir1_2;
	(void)argc;
	(void)argv;

	block1.cmd = "cd";
	block1.args = ft_split("cd ..", " ");
	// block1.redir = &redir1;
	// redir1.file = "out.txt";
	// redir1.type = REDIRECT_OUT;
	// redir1.next = &redir1_1;
	// redir1.next = NULL;
	// redir1_1.file = "srcs/cmd_handler.c";
	// redir1_1.type = REDIRECT_IN;
	// redir1_1.next = &redir1_2;
	// redir1_2.file = "boop";
	// redir1_2.type = REDIRECT_HEREDOC;
	// redir1_2.next = NULL;
	block1.next = NULL;
	block1.redir = NULL;
	// block1.next = &block2;
	// block2.cmd = "wc";
	// block2.args = ft_split("wc -l", " ");
	// block2.redir = NULL;
	// block2.next = NULL;
	cmd_handler(&block1, env);
	free(block1.args[0]);
	free(block1.args[1]);
	free(block1.args);
	// free(block2.args[0]);
	// free(block2.args[1]);
	// free(block2.args);
	return (0);
}
*/
