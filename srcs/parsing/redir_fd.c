/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 14:20:51 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/15 15:05:43 by masamoil         ###   ########.fr       */
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
		check_fd(fd);
	}
	return (fd);
}

int	get_fd_output(char *pathname, char *red)
{
	int	fd;

	fd = -1;
	if (ft_strcmp(red, ">") == 0)
		fd = open(pathname, O_CREAT | O_RDWR | O_TRUNC, 00664);
	else if (ft_strcmp (red, ">>") == 0)
		fd = open(pathname, O_CREAT | O_RDWR | O_APPEND, 00664);
	check_fd(fd);
	return (fd);
}

void	ft_redirection(char **str, t_params *params, t_data *data, t_token *token)
{
	int	i;
//	int	fds[2];

	(void)params;
	(void)data;

	token->fds[0] = 0;
	token->fds[1] = 1;
	i = 0;
	while (str[i])
	{
		if (str[i][0] == '>')
		{
			if (str[i][1] == '>')
				token->fds[1] = get_fd_output(&str[i][2], ">>");
			else
				token->fds[1] = get_fd_output(&str[i][1], ">");
		}
		if (str[i][0] == '<')
		{
			if (str[i][1] == '<')
			{
				ft_here_doc(&str[i][2], params, token);
			}
			else
				token->fds[0] = get_fd_input(&str[i][1], "<");
		}
		i++;
	}
	return ;
}
