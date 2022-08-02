/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 14:20:51 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/02 16:07:32 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	get_fd_input(char *pathname, char *red)
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
}

void	get_fd_output(char *pathname, char *red)
{
	int	fd;

	fd = -1;//(-1 ?!)
	if (ft_strcmp(red, ">") == 0)
		fd = open(pathname, O_CREAT | O_RDWR | O_TRUNC, 00664);
	else if (ft_strcmp (red, ">>") == 0)
		fd = open(pathname, O_CREAT | O_RDWR | O_APPEND, 00664);
	if (fd == -1)
		perror("Error:");
	else
	{
		if(dup2(fd, STDOUT_FILENO) == -1)
			perror("Error:");
		close(fd);
	}
}

void	ft_redirection(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i][0] == '>') 
		{
			if (str[i][1] == '>')
				get_fd_output(ft_strtrim(&str[i][2], " \t\n\v\f\r"), ">>");
			else
				get_fd_output(ft_strtrim(&str[i][1], " \t\n\v\f\r"), ">");
		}
		if (str[i][0] == '<') 
		{
			if (str[i][1] == '<')
				printf("heredoc\n");
			else
				get_fd_input(ft_strtrim(&str[i][1], " \t\n\v\f\r"), "<");
		}
		i++;
	}
}


