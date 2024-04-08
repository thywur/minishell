/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:37:52 by alermolo          #+#    #+#             */
/*   Updated: 2024/04/08 15:40:34 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <string.h>
# include <limits.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>

# define WORD 5
# define PIPE 6

# define REDIRECT_IN 1
# define REDIRECT_OUT 2
# define REDIRECT_APPEND 3
# define REDIRECT_HEREDOC 4

# define MALLOC_ERROR "malloc creation error"

typedef struct s_pipe
{
	char	**paths;
	int		cmd_count;
	pid_t	*pids;
	int		fd[4];
	int		has_heredoc;
	int		saved_stdin;
	int		saved_stdout;
}	t_pipe;

typedef struct s_redir
{
	char			*file;
	int				type;
	struct s_redir	*next;
}	t_redir;

typedef struct s_block
{
	char			*cmd;
	char			**args;
	t_redir			*redir;
	int				pid;
	int				heredoc_expansion;
	struct s_block	*next;
}	t_block;

typedef struct s_token
{
	int				type;
	int				error;
	char			*data;
	int				is_inquote;
	struct s_token	*next;
}					t_token;

typedef struct s_token_args
{
	char	quote;
	int		last_pos;
	int		pos;
}		t_token_args;

extern int	g_last_signal;

// -- EXECUTION
int		cmd_handler(t_block **blocks, char ***env);
void	combine_paths(char ***env, t_pipe *pipex, t_block *cmd_lst);
void	free_pipex(t_pipe *pipex);
void	free_arr(char **arr);
void	free_and_exit(t_pipe *pipex, t_block *blocks, char **env, int status);
int		exec_cmd(t_pipe *pipex, t_block *cmd_lst, char ***env);
void	err_msg(char *err);
void	joint_error_msg(char *err);
void	fd_error(char *filename);
void	redirect(t_pipe *pipex, t_block *cmd_lst, char ***env);

// -- BUILTINS
int		is_builtin(char *cmd);
int		exec_builtin(int *fd, t_block *cmd_lst, t_pipe *pipex, char ***env);
int		ft_echo(char **args, int fd[2]);
int		ft_cd(char **args, char ***env);
int		ft_env(char ***env, int fd[2]);
int		ft_exit(char **args, t_block *cmd_lst, t_pipe *pipex, char ***env);
int		ft_pwd(int fd[2]);
int		ft_export(char **args, char ***env, int fd[2]);
int		ft_unset(char **args, char ***env);

// -- BUILTIN UTILS
int		add_to_env_free(char *str, char ***env);
int		del_from_env(char *str, char ***env);
int		is_in_env(char *s, char **env);
int		replace_in_env(char *str, char ***env);
char	**sort_env(char **env);
char	**search_env(char *str, char **env);

// -- STRING UTILS
int		ft_strcmpr(char *s1, char *s2);
int		ft_contains(char *str, char c);
int		ft_startswith(char *big, char *little);
int		is_not_empty(char *str, int size);
char	*ft_strtrim(char *s1, char *set);
int		ft_strcpy(char *dst, const char *src);
int		ft_secured_strlen(char *str);
char	*expand_string(char *str, char **env);

// -- TOKEN UTILS
t_token	*add_token(t_token *token);
t_token	*create_empty_token(void);
void	trim_token_data(t_token *token);
int		get_final_data_size(char *data, char **env, int size, int exit_status);
int		is_inquote(char *data);

// -- MALLOC UTILS
char	*ft_strdup_size(char *str, int size);
void	ft_lst_delone(t_token **tokens, int index);

// -- REDIRECTIONS UTILS
void	ft_lstadd_back(t_redir **lst, t_redir *new);
t_redir	*create_redir(t_token *token, t_token *file_token, int *heredoc);
void	err_heredoc(char *limiter, int line_no);
char	*readline_heredoc(t_block *block, char **env);

// -- UTILS
void	print_error(char *error_descriptor);
int		ft_lstsize(t_block *lst);
t_redir	*ft_lstlast(t_redir *lst);
void	get_final_data_size_extra(char *quote, char car, int *size);
void	expand_token_extra(char *quote, char car, char *word, int *j);

// -- FREE LINKED CHAINS
void	free_tokens(t_token **tokens);
void	free_blocks(t_block **blocks);
void	free_redir(t_redir **redir);
void	free_single_redir(t_redir **redir);

// -- TOKENS
int		check_tokens(t_token **token);
t_token	*save_token(t_token *token, char *cmdline, t_token_args *args);
t_token	*split_cmdline_into_tokens(char *cmdline);
int		expand_tokens(t_token **tokens, char **env, int exit_status);
char	*expand_token(char *data, char **env, int size, int exit_status);

// -- BLOCKS
t_block	*join_tokens_into_blocks(t_token **tokens);

// -- SIGNALS
void	sig_handler_main(int sig);
void	sig_handler_child(int sig);
void	sig_handler_heredoc(int sig);
void	launch_heredoc_sig_catcher(void);
void	launch_child_sig_catcher(void);

#endif
