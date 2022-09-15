/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 16:05:49 by masamoil          #+#    #+#             */
/*   Updated: 2022/09/15 15:52:02 by masamoil         ###   ########.fr       */
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

// used to make dup2 and close the old fd
static void	dup_and_close(int fd_close, int fd)
{
	dup2(fd_close, fd);
	close(fd_close);
}

// execution if only one command
static int	only_one(t_token *token, t_params *params)
{
	int			old_fd[2];

	old_fd[0] = dup(0);
	old_fd[1] = dup(1);
	if (token->fds[0] > 0)
		dup_and_close(token->fds[0], 0);
	if (token->fds[1] != 1 && token->fds[1] >= 0)
		dup_and_close(token->fds[1], 1);
	ft_select_builtin(token, params, 0, old_fd);
	if (token->fds[0] > 0)
		close(token->fds[0]);
	if (token->fds[0] != 1 && token->fds[1] >= 0)
		close(token->fds[1]);
	dup_and_close(old_fd[0], 0);
	dup_and_close(old_fd[1], 1);
	if (errno == 12)
		return (-1);
	return (0);
}

// check fd anf close
static int	close_fds(int zero, int one)
{
	if (zero > 0)
		close(zero);
	if (one != 1 && one >= 0)
		close(one);
	return (-1);
}

// start the execution by making pipes and children
int	ft_execute(t_token *token, t_params *params)
{
	int			nbr;
	int			nbr2;

	if (token->next == NULL && token->prev == NULL)
		return (only_one(token, params));
	nbr = ft_size(token);
	params->data->pid = malloc(sizeof(int) * nbr);
	if (params->data->pid == NULL)
		return (set_error_malloc("creating child\n"));
	params->data->pipe_fd = malloc(sizeof(t_pipe_fd) * nbr);
	if (params->data->pipe_fd == NULL)
	{
		free(params->data->pid);
		close_fds(token->fds[0], token->fds[1]);
		return (set_error_malloc("creating child\n"));
	}
	nbr2 = nbr;
	while (--nbr >= 0)
		if (create_pipe(params->data->pipe_fd,
				params->data->pid, nbr, nbr2) == -1)
			return (close_fds(token->fds[0], token->fds[1]));
	ft_pipe(token, params, params->data->pid, params->data->pipe_fd);
	free(params->data->pid);
	free(params->data->pipe_fd);
	return (g_exit_st);
}
