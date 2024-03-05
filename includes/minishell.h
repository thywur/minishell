/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:37:52 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/05 12:31:41 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>				// a virer
// # include <unistd.h>
// # include <stdlib.h>
// # include <sys/types.h>
# include <sys/wait.h>
# include <stdarg.h>
# include <string.h>
# include <limits.h>

# define REDIRECT_IN 1
# define REDIRECT_OUT 2
# define REDIRECT_APPEND 3
# define REDIRECT_HEREDOC 4

typedef struct s_pipe
{
	// int		in_fd;
	// int		out_fd;
	// int		has_heredoc;
	char	**paths;
	// char	***cmds;
	int		cmd_count;
	pid_t	*pids;
	int		fd[4];
}	t_pipe;

typedef struct s_redir
{
	char	*file;
	int		type;
	struct s_redir	*next;
}	t_redir;

typedef struct	s_block
{
	char	*cmd;
	char	**args;
	t_redir	*redir;
	int		pid;
	struct s_block	*next;
}	t_block;

void	combine_paths(char **env, t_pipe *pipex, t_block *cmd_lst);
void	free_struct(t_pipe *pipex);
void	free_arr(char **arr);
void	free_and_exit(t_pipe *pipex, int exit_status);
void	parse_cmds(int argc, char **argv, t_pipe *pipex);
int		exec_cmd(t_pipe *pipex, t_block *cmd_lst, char **env);
void	err_msg(char *err);
void	joint_error_msg(char *err);
void	fd_error(char *filename);
int		ft_lstsize(t_block *lst);
t_redir	*ft_lstlast(t_redir *lst);

#endif
