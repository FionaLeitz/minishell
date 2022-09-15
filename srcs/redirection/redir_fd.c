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

// open for input redirection
int	get_fd_input(char *pathname, char *red)
{
	int	fd;

	fd = 0;
	if (ft_strcmp(red, "<") == 0)
	{
		fd = open(pathname, O_RDONLY);
		if (check_fd(fd, pathname) == -1)
			return (-1);
	}
	return (fd);
}

// open for output redirection
static int	get_fd_output(char *pathname, char *red)
{
	int	fd;

	fd = -1;
	if (ft_strcmp(red, ">") == 0)
		fd = open(pathname, O_CREAT | O_RDWR | O_TRUNC, 00664);
	else if (ft_strcmp (red, ">>") == 0)
		fd = open(pathname, O_CREAT | O_RDWR | O_APPEND, 00664);
	if (check_fd(fd, pathname) == -1)
		return (-1);
	return (fd);
}

// if redirection to the right
static void	right_redir(char **str, int *i, t_token *token)
{
	if (str[*i][0] == '>')
	{
		if (token->fds[1] != 1 && token->fds[1] != -1)
			close(token->fds[1]);
		if (str[*i][1] == '>')
			token->fds[1] = get_fd_output(&str[*i][2], ">>");
		else
			token->fds[1] = get_fd_output(&str[*i][1], ">");
	}
}

// if redirection to the left
static void	left_redir(char **str, int *i, t_token *token, t_params *params)
{
	if (str[*i][0] == '<')
	{
		if (token->fds[0] > 0)
			close(token->fds[0]);
		if (str[*i][1] == '<')
		{
			if (params->error != -2)
			{
				g_exit_st = 0;
				params->old_fd[0] = dup(0);
				token->fds[0] = ft_here_doc(&str[*i][2], params);
				if (token->fds[0] == -2)
					params->error = -2;
				if (params->old_fd[0] != -1)
					close(params->old_fd[0]);
			}
		}
		else
			token->fds[0] = get_fd_input(&str[*i][1], "<");
	}
}

// open
void	ft_redirection(char **str, t_params *params, t_token *token)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		right_redir(str, &i, token);
		left_redir(str, &i, token, params);
		if (token->fds[0] == -1 || token->fds[1] == -1)
			break ;
	}
}
