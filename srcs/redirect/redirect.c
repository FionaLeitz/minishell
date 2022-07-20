/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 10:17:45 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/20 11:46:49 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	get_red_fd(char *pathname, char *red)
{
	int	fd;
	
	if (ft_strcmp(red, "<") == 0)
	{
		fd = open(pathname, O_RDONLY);
		if (fd == -1)
			perror("Error:");
		else
		{
			if (dup2(fd, STDIN_FILENO) == -1)
				perror("Error:");
			close(fd);
		}
	}
	else
	{
		if (ft_strcmp(red, ">") == 0)
			fd = open(pathname, O_CREAT | O_RDWR | O_TRUNC, 0644);
		else if (ft_strcmp (red, ">>") == 0)
			fd = open(pathname, O_CREAT | O_RDWR | 0_APPEND, 0644);
		if (fd == -1)
			perror("Error:");
		else
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
				perror("Error:");
			close(fd);
		}
	}
}
