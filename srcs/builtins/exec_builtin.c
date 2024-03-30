/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 15:09:10 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/29 16:18:43 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	else if (!ft_strcmp(cmd, "echo"))
		return (1);
	else if (!ft_strcmp(cmd, "cd"))
		return (1);
	else if (!ft_strcmp(cmd, "pwd"))
		return (1);
	else if (!ft_strcmp(cmd, "export"))
		return (1);
	else if (!ft_strcmp(cmd, "unset"))
		return (1);
	else if (!ft_strcmp(cmd, "env"))
		return (1);
	else if (!ft_strcmp(cmd, "exit"))
		return (1);
	else
		return (0);
}

int	exec_builtin(int *fd, t_block *cmd_lst, t_pipe *pipex, char ***env)
{
	int		status;

	status = 0;
	fd[2] = STDIN_FILENO;
	fd[3] = STDOUT_FILENO;
	if (cmd_lst->redir)
		redirect(pipex, cmd_lst, env);
	if (ft_strcmp(cmd_lst->cmd, "echo") == 0)
		status = ft_echo(cmd_lst->args, &fd[2]);
	else if (ft_strcmp(cmd_lst->cmd, "cd") == 0)
		status = ft_cd(cmd_lst->args, env);
	else if (!ft_strcmp(cmd_lst->cmd, "pwd"))
		status = ft_pwd(&fd[2]);
	else if (!ft_strcmp(cmd_lst->cmd, "export"))
		status = ft_export(cmd_lst->args, env, &fd[2]);
	else if (!ft_strcmp(cmd_lst->cmd, "unset"))
		status = ft_unset(cmd_lst->args, env);
	else if (!ft_strcmp(cmd_lst->cmd, "env"))
		status = ft_env(env, &fd[2]);
	else if (!ft_strcmp(cmd_lst->cmd, "exit"))
		status = ft_exit(cmd_lst->args, cmd_lst, pipex, env);
	return (status);
}
