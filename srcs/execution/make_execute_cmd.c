/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_execute_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 16:10:18 by masamoil          #+#    #+#             */
/*   Updated: 2022/09/09 16:10:32 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// execute command and write error if does not work
static int	execute_command(t_token *token, t_params *params, int *old_fd)
{
	ft_signals(COMMAND);
	get_path(token->args, token, params, old_fd);
	if (errno == 12)
		return (-1);
	execve(token->args[0], token->args, params->env);
	write(2, "minishell: ", 11);
	write(2, token->args[0], ft_strlen(token->args[0]));
	write(2, ": command not found\n", 20);
	g_exit_st = 127;
	free_command_no(params, old_fd);
	return (0);
}

// execve if not built-in
void	make_command(t_token *token, t_params *params, int i, int *old_fd)
{
	int	pid;
	int	status;

	pid = 0;
	status = 0;
	if (i == 0)
	{
		ft_signals(MUTE);
		pid = fork();
		if (check_child(pid) == -1)
			return ;
	}
	if (pid == 0)
		if (execute_command(token, params, old_fd) == -1)
			return ;
	if (i == 0)
	{
		if (0 < waitpid(pid, &g_exit_st, 0) && (WIFEXITED(g_exit_st)))
			g_exit_st = WEXITSTATUS(g_exit_st);
		else if (WIFSIGNALED(g_exit_st))
			g_exit_st = 128 + WTERMSIG(g_exit_st);
		check_exit_status();
	}
	ft_signals(DEFAULT);
}
