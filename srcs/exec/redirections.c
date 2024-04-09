/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quteriss <quteriss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 15:43:28 by alermolo          #+#    #+#             */
/*   Updated: 2024/04/09 14:43:58 by quteriss         ###   ########.fr       */
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

static void	create_heredoc(t_pipe *pipex, t_block *block, char **env)
{
	char	*line;
	char	*limiter;
	int		line_no;

	launch_heredoc_sig_catcher();
	pipex->fd[2] = open(".here_doc", O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (pipex->fd[2] < 0)
		joint_error_msg(".here_doc");
	line_no = 1;
	limiter = ft_strjoin(block->redir->file, "\n");
	line = readline_heredoc(block, env, limiter);
	while (line && ft_strcmp(line, limiter) != 0 && g_status != 130)
	{
		write(pipex->fd[2], line, ft_strlen(line));
		free(line);
		line = readline_heredoc(block, env, limiter);
		line_no++;
	}
	if (g_status == 130)
		pipex->has_heredoc = 2;
	if (!line && g_status == 0)
		err_heredoc(limiter, line_no);
	free(line);
	free(limiter);
	close(pipex->fd[2]);
}

int	redirect(t_pipe *pipex, t_block *cmd_lst, char ***env)
{
	pipex->has_heredoc = 0;
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
			pipex->has_heredoc = 1;
			g_status = 0;
			create_heredoc(pipex, cmd_lst, *env);
			pipex->fd[2] = open(".here_doc", O_RDONLY);
		}
		if (pipex->fd[2] == -1 || pipex->fd[3] == -1)
			return (fd_error(cmd_lst->redir->file), 1);
		free_single_redir(&cmd_lst->redir);
	}
	return (0);
}
