/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 10:53:47 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/08 10:03:32 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// handle ctrl-\ and ctrl-c
void	ft_sig_manage(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}		
	if (signal == SIGQUIT)
	{
		write(1, "\b\b \b\b", 6);
		return ;
	}
}

void	ft_manage_sig(void)
{
	signal(SIGINT, ft_sig_manage);
	signal(SIGQUIT, ft_sig_manage);
}

// handle ctrl-d
void	ft_exit_d(t_data *data, t_params *params)
{
	write(1, "exit\n", 4);
	free_params(params);
	free_struct(data);
	exit(0);
}

//handle signals in the here_doc
void	ft_sig_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		close(STDIN_FILENO);
	}
	if (signal == SIGQUIT)
		write(1, "\b\b \b\b", 6);
//	if (signal == SIGSEGV)
//	{
//		write(1, "n", 1);
//		exit(0);
//	}
}

void	ft_manage_sighd(void)
{
	signal(SIGINT, ft_sig_heredoc);
	signal(SIGQUIT, ft_sig_heredoc);
}
