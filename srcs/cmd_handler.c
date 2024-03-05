/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 16:05:07 by alermolo          #+#    #+#             */
/*   Updated: 2024/03/05 12:34:09 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_struct(t_pipe *pipex, t_block *cmd_lst)
{
	// pipex->in_fd = open(argv[1], O_RDONLY);
	// if (pipex->in_fd == -1)
	// 	fd_error(argv[1]);
	pipex->paths = NULL;
	// pipex->cmds = NULL;
	pipex->cmd_count = ft_lstsize(cmd_lst);
	// pipex->has_heredoc = 0;
	// pipex->out_fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	pipex->pids = malloc(sizeof(int) * pipex->cmd_count);
	if (!pipex->pids)
		free_and_exit(pipex, EXIT_FAILURE);
}

// static void	read_heredoc(int argc, char **argv, t_pipe *pipex)
// {
// 	char	*line;
// 	char	*limiter;

// 	if (argc < 6)
// 	{
// 		write (2, "expected format: here_doc LIMITER cmd1 cmd2 outfile\n", 52);
// 		exit(-1);
// 	}
// 	pipex->has_heredoc = 1;
// 	pipex->cmd_count = argc - 4;
// 	pipex->in_fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, 0777);
// 	if (pipex->in_fd < 0)
// 		joint_error_msg(argv[1]);
// 	pipex->out_fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_APPEND, 0644);
// 	line = get_next_line(0);
// 	limiter = ft_strjoin(argv[2], "\n");
// 	while (line && ft_strcmp(line, limiter) != 0)
// 	{
// 		write(pipex->in_fd, line, ft_strlen(line));
// 		free(line);
// 		line = get_next_line(0);
// 	}
// 	free(line);
// 	free(limiter);
// 	close(pipex->in_fd);
// }

int	cmd_handler(t_block *cmd_lst, char **env)
{
	t_pipe	pipex;
	int		status;

	// if (ft_strcmp(argv[1], "here_doc") == 0)
	// 	read_heredoc(argc, argv, &pipex);
	init_struct(&pipex, cmd_lst);
	// parse_cmds(argc, argv, &pipex);
	combine_paths(env, &pipex, cmd_lst);
	status = exec_cmd(&pipex, cmd_lst, env);
	// if (pipex.out_fd == -1)
	// 	fd_error(argv[argc - 1]);
	free_struct(&pipex);
	return (status);
}

int	main(int argc, char **argv, char **env)
{
	t_block	block1;
	t_block block2;
	t_redir	redir1;
	t_redir redir1_1;
	t_redir	redir1_2;
	(void)argc;
	(void)argv;

	block1.cmd = "cat";
	block1.args = ft_split("cat", " ");
	block1.redir = &redir1;
	redir1.file = "Makefile";
	redir1.type = REDIRECT_IN;
	redir1.next = &redir1_1;
	redir1_1.file = "srcs/cmd_handler.c";
	redir1_1.type = REDIRECT_IN;
	redir1_1.next = &redir1_2;
	redir1_2.file = "srcs/cmd_parsing.c";
	redir1_2.type = REDIRECT_IN;
	redir1_2.next = NULL;
	block1.next = &block2;
	block2.cmd = "wc";
	block2.args = ft_split("wc -l", " ");
	block2.redir = NULL;
	block2.next = NULL;
	cmd_handler(&block1, env);
	free(block1.args[0]);
	free(block1.args[1]);
	free(block1.args);
	free(block2.args[0]);
	free(block2.args[1]);
	free(block2.args);
	return (0);
}

// int	main(int argc, char **argv, char **env)
// {
// 	t_pipe	pipex;
// 	int		status;

// 	if (argc < 5)
// 	{
// 		write (2, "expected format: infile cmd1 cmd2 outfile\n", 42);
// 		exit(-1);
// 	}
// 	if (ft_strcmp(argv[1], "here_doc") == 0)
// 		read_heredoc(argc, argv, &pipex);
// 	init_struct(argc, argv, &pipex);
// 	parse_cmds(argc, argv, &pipex);
// 	combine_paths(env, &pipex);
// 	status = exec_cmd(&pipex, env);
// 	if (pipex.out_fd == -1)
// 		fd_error(argv[argc - 1]);
// 	free_struct(&pipex);
// 	return (status);
// }
