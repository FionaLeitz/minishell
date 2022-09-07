/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 16:05:49 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/26 17:12:55 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// create the pipes for every child
static int	create_pipe(t_pipe_fd *pipe_fd, int *pid, int nbr, int nbr2)
{
	if (pipe(pipe_fd[nbr].raw) == -1)
	{
		free(pid);
		while (--nbr2 > nbr)
		{
			close(pipe_fd[nbr2].raw[0]);
			close(pipe_fd[nbr2].raw[1]);
		}
		free(pipe_fd);
		return (set_error_malloc("creating child\n"));
	}
	return (0);
}

// execution if only one command
static int	only_one(t_token *token, t_params *params)
{
	int			old_fd[2];

	old_fd[0] = dup(0);
	old_fd[1] = dup(1);
	if (token->fds[0] != 0)
	{
		dup2(token->fds[0], 0);
		close(token->fds[0]);
	}
	if (token->fds[1] != 1)
	{
		dup2(token->fds[1], 1);
		close(token->fds[1]);
	}
	ft_select_builtin(token, params, 0, old_fd);
	if (token->fds[0] != 0)
		close(token->fds[0]);
	if (token->fds[0] != 1)
		close(token->fds[1]);
	dup2(old_fd[0], 0);
	dup2(old_fd[1], 1);
	close(old_fd[0]);
	close(old_fd[1]);
	if (errno == 12)
		return (-1);
	return (0);
}

// start the execution by making pipes and children
int	ft_execute(t_token *token, t_params *params)
{
	int			nbr;
	int			nbr2;
	int			*pid;
	t_pipe_fd	*pipe_fd;

	if (token->next == NULL && token->prev == NULL)
		return (only_one(token, params));
	nbr = ft_size(token);
	pid = malloc(sizeof(int) * nbr);
	if (pid == NULL)
		set_error_malloc("creating child\n");
	pipe_fd = malloc(sizeof(t_pipe_fd) * nbr);
	if (pipe_fd == NULL)
	{
		free(pid);
		if (token->fds[0] != 0)
			close(token->fds[0]);
		if (token->fds[0] != 1)
			close(token->fds[1]);
		return (set_error_malloc("creating child\n"));
	}
	nbr2 = nbr;
	while (--nbr >= 0)
	{
		if (create_pipe(pipe_fd, pid, nbr, nbr2) == -1)
		{
			if (token->fds[0] != 0)
				close(token->fds[0]);
			if (token->fds[0] != 1)
				close(token->fds[1]);
			return (-1);
		}
	}
	ft_pipe(token, params, pid, pipe_fd);
	if (token->fds[0] != 0)
		close(token->fds[0]);
	if (token->fds[0] != 1)
		close(token->fds[1]);
	free(pid);
	free(pipe_fd);
	return (g_exit_st);
}
