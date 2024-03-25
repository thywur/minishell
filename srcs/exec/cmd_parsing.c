/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:46:35 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/25 15:03:51 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	parse_cmds(t_pipe *pipex, t_block *cmd_lst)
// {
// 	int		i;
// 	// int		j;

// 	i = 0;
// 	j = 0;
// 	pipex->cmds = malloc(sizeof(char **) * (pipex->cmd_count + 1));
// 	if (!pipex->cmds)
// 		free_and_exit(pipex, EXIT_FAILURE);
// 	while (cmd_lst)
// 	{
// 		// pipex->cmds[j] = ft_split(argv[i], " \t\v\n\f\r");
// 		pipex->cmds[i]
// 		i++;
// 		j++;
// 	}
// 	pipex->cmds[j] = 0;
// }

static char	**parse_paths(char ***env)
{
	char	**paths;
	int		i;

	i = 0;
	paths = NULL;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], "PATH=", 5) == 0)
			paths = ft_split(&(*env)[i][5], ":");
		i++;
	}
	return (paths);
}

static char	*join_path(char *path, char *command)
{
	char	*res;
	char	*temp;

	temp = ft_strjoin(path, "/");
	res = ft_strjoin(temp, command);
	free(temp);
	return (res);
}

static char	*cmd_path(char **paths, char *cmd, int i)
{
	char	*cmd_path;

	if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	if (paths)
	{
		while (paths[i])
		{
			cmd_path = join_path(paths[i++], cmd);
			if (access(cmd_path, F_OK) == 0 && access(cmd_path, X_OK) == 0)
				return (cmd_path);
			if ((!access(cmd_path, F_OK) && access(cmd_path, X_OK)) || !cmd)
			{
				free(cmd_path);
				cmd_path = ft_strjoin("permission denied: ", cmd);
				err_msg(cmd_path);
				return (NULL);
			}
			free(cmd_path);
		}
	}
	cmd_path = ft_strjoin("command not found: ", cmd);
	err_msg(cmd_path);
	return (NULL);
}

void	combine_paths(char ***env, t_pipe *pipex, t_block *cmd_lst)
{
	char	**env_paths;
	int		i;

	env_paths = parse_paths(env);
	pipex->paths = malloc(sizeof(char *) * (pipex->cmd_count + 1));
	if (!pipex->paths)
		free_and_exit(pipex, cmd_lst, *env, EXIT_FAILURE);
	i = 0;
	while (i < pipex->cmd_count && cmd_lst)
	{
		if (is_builtin(cmd_lst->cmd))
			pipex->paths[i] = NULL;
		else
			pipex->paths[i] = cmd_path(env_paths, cmd_lst->cmd, 0);
		cmd_lst = cmd_lst->next;
		i++;
	}
	if (env_paths)
		free_arr(env_paths);
	pipex->paths[i] = 0;
}

//blep
