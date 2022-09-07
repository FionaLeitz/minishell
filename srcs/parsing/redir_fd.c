/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 14:20:51 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/18 11:41:04 by masamoil         ###   ########.fr       */
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
		check_fd(fd, pathname);
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
	check_fd(fd, pathname);
	return (fd);
}

void	ft_redirection(char **str, t_params *params, t_token *token)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i][0] == '>')
		{
			if (token->fds[1] != 1)
				close(token->fds[1]);
			if (str[i][1] == '>')
				token->fds[1] = get_fd_output(&str[i][2], ">>");
			else
				token->fds[1] = get_fd_output(&str[i][1], ">");
		}
		if (str[i][0] == '<')
		{
			if (token->fds[0] != 0)
				close(token->fds[0]);
//			dprintf(2, "delim = -%s-\n", &str[i][2]);
			if (str[i][1] == '<')
				token->fds[0] = ft_here_doc(&str[i][2], params);
			else
				token->fds[0] = get_fd_input(&str[i][1], "<");
		}
		if (token->fds[0] == -1 || token->fds[1] == -1)
			return ;
	}
}
