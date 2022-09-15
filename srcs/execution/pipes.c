/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 15:28:05 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/26 16:18:40 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// use dup2 to duplicate fds and read and write at the right place
static void	make_dup(t_token *token, t_pipe_fd *pipe_fd, int i)
{
	close((pipe_fd[i].raw[0]));
	if (i != 0 && token->fds[0] == 0)
	{
		dup2(pipe_fd[i - 1].raw[0], 0);
		close((pipe_fd[i - 1].raw[0]));
	}
	else if (token->fds[0] != 0)
	{
		dup2(token->fds[0], 0);
		close((token->fds[0]));
		if (i != 0)
			close((pipe_fd[i - 1].raw[0]));
	}
	if (token->fds[1] != 1)
	{
		dup2(token->fds[1], 1);
		close((token->fds[1]));
	}
	else if (token->next != NULL)
	{
		dup2(pipe_fd[i].raw[1], 1);
		close((pipe_fd[i].raw[1]));
	}
}

// what happens in child
static void	in_child(t_token *tok, t_params *par, t_pipe_fd *pipe_fd, int i)
{
	t_token	*new;

	ft_signals(COMMAND);
	make_dup(tok, pipe_fd, i);
	new = tok;
	while (new)
	{
		close((pipe_fd[i].raw[0]));
		close((pipe_fd[i].raw[1]));
		i++;
		new = new->next;
	}
	ft_select_builtin(tok, par, 1, NULL);
}

// clean if execve is not executed
static void	clean_child(t_params *params, int *pid, t_pipe_fd *pipe_fd)
{
	t_token	*token;
	t_token	*tmp;

	token = params->data->head;
	free_params(params);
	free(pid);
	free(pipe_fd);
	free(params->data->trimmed);
	while (token != NULL)
	{
		tmp = token;
		if (tmp->fds[0] > 0)
			close(tmp->fds[0]);
		if (tmp->fds[1] != 1 && tmp->fds[1] >= 0)
			close(tmp->fds[1]);
		free(tmp->value);
		free_table(tmp->args);
		free_table(tmp->red);
		token = token->next;
		free(tmp);
	}
	exit(g_exit_st);
}

// get exit status from child
static void	get_exit_st(int i, int *pid)
{
	int	save;

	save = 0;
	while (save <= i)
	{
		if (0 < waitpid(pid[save++], &g_exit_st, 0) && (WIFEXITED(g_exit_st)))
			g_exit_st = WEXITSTATUS(g_exit_st);
		else if (WIFSIGNALED(g_exit_st))
			g_exit_st = 128 + WTERMSIG(g_exit_st);
		else if (WIFSTOPPED(g_exit_st))
			g_exit_st = 128 + WSTOPSIG(g_exit_st);
	}
}

// create childs
int	ft_pipe(t_token *token, t_params *params, int *pid, t_pipe_fd *pipe_fd)
{
	int	i;

	i = -1;
	while (token)
	{
		ft_signals(MUTE);
		pid[++i] = fork();
		if (check_child(pid[i]) == -1)
			return (-1);
		if (pid[i] == 0)
		{
			in_child(token, params, pipe_fd, i);
			clean_child(params, pid, pipe_fd);
		}
		close(pipe_fd[i].raw[1]);
		if (i != 0)
			close((pipe_fd[i - 1].raw[0]));
		if (token->next == NULL)
			close((pipe_fd[i].raw[0]));
		token = token->next;
	}
	get_exit_st(i, pid);
	check_exit_status();
	ft_signals(DEFAULT);
	return (0);
}
