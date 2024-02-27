/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:37:52 by alermolo          #+#    #+#             */
/*   Updated: 2024/01/16 14:31:18 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdarg.h>
# include <string.h>
# include <limits.h>

typedef struct s_pipe
{
	int		in_fd;
	int		out_fd;
	int		has_heredoc;
	char	**paths;
	char	***cmds;
	int		cmd_count;
	pid_t	*pids;
	int		fd[4];
}	t_pipe;

char	**ft_split(char *str, char *charset);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	ft_strlen(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoin_free(char const *s1, char const *s2);
void	combine_paths(char **env, t_pipe *pipex);
void	free_struct(t_pipe *pipex);
void	free_arr(char **arr);
void	free_and_exit(t_pipe *pipex, int exit_status);
void	parse_cmds(int argc, char **argv, t_pipe *pipex);
int		exec_cmd(t_pipe *pipex, char **env);
void	err_msg(char *err);
void	joint_error_msg(char *err);
void	fd_error(char *filename);
char	*get_next_line(int fd);
void	*free_and_null(char *s);
char	*ft_strdup(const char *s);
char	*ft_substr_gnl(char *s, unsigned int start, size_t len);
void	*ft_bzero(void *s, size_t n);

#endif
