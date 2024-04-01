/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 15:43:07 by alermolo          #+#    #+#             */
/*   Updated: 2024/04/01 16:10:45 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	err_putchar(char c)
{
	write(STDERR_FILENO, &c, 1);
}

void	err_putnbr(unsigned int nb)
{
	if (nb > 9)
	{
		err_putnbr(nb / 10);
		err_putnbr(nb % 10);
	}
	else
		err_putchar(nb + 48);
}

void	err_heredoc(char *limiter, int line_no)
{
	size_t	i;

	write(2, "minishell : warning: here-document at line ", 43);
	err_putnbr(line_no);
	write(2, " delimited by end-of-file (wanted '", 35);
	i = 0;
	while (i < ft_strlen(limiter) - 1)
		err_putchar(limiter[i++]);
	write(2, "')\n", 3);
}

void	err_msg(char *err)
{
	write(2, err, ft_strlen(err));
	write(2, "\n", 1);
	free(err);
}

void	joint_error_msg(char *err)
{
	char	*msg;
	char	*temp;

	temp = ft_strjoin(strerror(errno), ": ");
	msg = ft_strjoin(temp, err);
	err_msg(msg);
	free(temp);
}

void	fd_error(char *filename)
{
	char	*msg;

	if (access(filename, F_OK) != 0)
	{
		msg = ft_strjoin("no such file or directory: ", filename);
		err_msg(msg);
	}
	else if (access(filename, X_OK) != 0)
	{
		msg = ft_strjoin("permission denied: ", filename);
		err_msg(msg);
	}
	else
		joint_error_msg(filename);
}
