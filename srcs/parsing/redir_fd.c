/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 14:20:51 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/25 14:21:12 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	get_fd_input(char *pathname, char *red)
{
	int	fd;

	fd = 0;

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
	return(fd);
}

int	get_fd_output(char*pathname, char *red)
{
	int	fd;

	fd = 0;//(-1 ?!)
	if (ft_strcmp(red, ">") == 0)
		fd = open(pathname, O_CREAT | O_RDWR | O_TRUNC, 0644);
	else if (ft_strcmp (red, ">>") == 0)
		fd = open(pathname, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd == -1)
		perror("Error:");
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			perror("Error:");
		close(fd);
	}
	return (fd);
}

void	ft_redirection(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_strcmp(str[i], ">") == 0)
			output_create
		else if (ft_strcmp(str[i], ">>") == 0)
			output_append
		else if (ft_strcmp(str[i], "<") == 0)
			read
		else if(ft_strcmp(str[i], "<<") == 0)
			heredoc
	}
	i++;
}
