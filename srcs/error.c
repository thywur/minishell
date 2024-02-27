/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 15:43:07 by alermolo          #+#    #+#             */
/*   Updated: 2024/01/16 14:26:30 by alermolo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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
