/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 10:53:47 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/18 16:34:03 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// handles ctrl-c in the shell with prompt
void	ft_sig_manage(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		exit_st = 130;
	}		
}

//handles ctrl-c in heredoc
void	ft_sig_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		//rl_replace_line("", 0);
		close(STDIN_FILENO);
		exit_st = 130;
	}
}

void	ft_signals(t_sig_mode mode)
{
	
	if (mode == DEFAULT)
	{
		signal(SIGINT, ft_sig_manage);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == HEREDOC)
	{	
		signal(SIGINT, ft_sig_heredoc);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == MUTE)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == RESET)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
}

// handles ctrl-d
void	ft_exit_d(t_data *data, t_params *params)
{
	write(1, "exit\n", 4);
	free_params(params);
	free_struct(data);
	exit(0);
}

