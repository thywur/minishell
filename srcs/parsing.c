/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:46:35 by alermolo          #+#    #+#             */
/*   Updated: 2024/01/16 14:27:35 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	parse_cmds(int argc, char **argv, t_pipe *pipex)
{
	int		i;
	int		j;

	i = 2;
	j = 0;
	if (pipex->has_heredoc)
		i = 3;
	pipex->cmds = malloc(sizeof(char **) * (pipex->cmd_count + 1));
	if (!pipex->cmds)
		free_and_exit(pipex, EXIT_FAILURE);
	while (i < argc - 1)
	{
		pipex->cmds[j] = ft_split(argv[i], " \t\v\n\f\r");
		i++;
		j++;
	}
	pipex->cmds[j] = 0;
}

static char	**parse_paths(char **env)
{
	char	**paths;
	int		i;

	i = 0;
	paths = NULL;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			paths = ft_split(&env[i][5], ":");
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

void	combine_paths(char **env, t_pipe *pipex)
{
	char	**env_paths;
	int		i;

	env_paths = parse_paths(env);
	pipex->paths = malloc(sizeof(char *) * (pipex->cmd_count + 1));
	if (!pipex->paths)
		free_and_exit(pipex, EXIT_FAILURE);
	i = 0;
	if (pipex->in_fd < 0)
	{
		pipex->paths[0] = NULL;
		i = 1;
	}
	while (i < pipex->cmd_count)
	{
		if (i == pipex->cmd_count - 1 && pipex->out_fd == -1)
			pipex->paths[i] = 0;
		else
			pipex->paths[i] = cmd_path(env_paths, pipex->cmds[i][0], 0);
		i++;
	}
	if (env_paths)
		free_arr(env_paths);
	pipex->paths[i] = 0;
}
