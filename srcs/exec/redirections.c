/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 15:43:28 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/30 16:13:19 by quteriss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	redir_in(t_pipe *pipex, t_block *cmd_lst)
{
	if (pipex->fd[2] > 0)
		close(pipex->fd[2]);
	pipex->fd[2] = open(cmd_lst->redir->file, O_RDONLY);
}

static void	redir_out(t_pipe *pipex, t_block *cmd_lst)
{
	close(pipex->fd[3]);
	pipex->fd[3] = open(cmd_lst->redir->file, O_CREAT | O_RDWR | O_TRUNC, 0644);
}

static void	redir_append(t_pipe *pipex, t_block *cmd_lst)
{
	close(pipex->fd[3]);
	pipex->fd[3] = open(cmd_lst->redir->file,
		O_CREAT | O_RDWR | O_APPEND, 0644);
}

static void	create_heredoc(t_pipe *pipex, t_redir *redir)
{
	char	*line;
	char	*limiter;

	pipex->fd[2] = open("here_doc", O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (pipex->fd[2] < 0)
		joint_error_msg("here_doc");
	write(2, "> ", 2);
	line = get_next_line(0);
	limiter = ft_strjoin(redir->file, "\n");
	signal_hub(3);
	while (line && ft_strcmp(line, limiter) != 0)
	{
		write(2, "> ", 2);
		write(pipex->fd[2], line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	free(line);
	free(limiter);
	close(pipex->fd[2]);
}

void	redirect(t_pipe *pipex, t_block *cmd_lst, char ***env)
{
	while (cmd_lst->redir)
	{
		if (cmd_lst->redir->type == REDIRECT_IN)
			redir_in(pipex, cmd_lst);
		else if (cmd_lst->redir->type == REDIRECT_OUT)
			redir_out(pipex, cmd_lst);
		else if (cmd_lst->redir->type == REDIRECT_APPEND)
			redir_append(pipex, cmd_lst);
		else if (cmd_lst->redir->type == REDIRECT_HEREDOC)
		{
			if (pipex->fd[2] > 0)
				close(pipex->fd[2]);
			create_heredoc(pipex, cmd_lst->redir);
			pipex->fd[2] = open("here_doc", O_RDONLY);
		}
		if (pipex->fd[2] == -1 || pipex->fd[3] == -1)
		{
			perror(NULL);
			free_and_exit(pipex, cmd_lst, *env, EXIT_FAILURE);
		}
		free_single_redir(&cmd_lst->redir);
	}
}
