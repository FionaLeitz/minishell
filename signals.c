/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 10:53:47 by masamoil          #+#    #+#             */
/*   Updated: 2022/09/13 09:28:19 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// handles ctrl-c in the shell with prompt
void	ft_sig_int(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_st = 130;
	}
}

//handles ctrl-c in heredoc
void	ft_sig_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		if (STDIN_FILENO != -1)
			close(STDIN_FILENO);
		g_exit_st = 130;
	}
}

//handles ctrl'\'
void	ft_sig_quit(int signal)
{
	if (signal == SIGQUIT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		g_exit_st = 131;
	}
}

//handles the signals depending on mode
void	ft_signals(t_sig_mode mode)
{
	if (mode == DEFAULT)
	{
		signal(SIGINT, ft_sig_int);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTERM, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
	}
	else if (mode == HEREDOC)
	{	
		signal(SIGINT, ft_sig_heredoc);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
	}
	else if (mode == MUTE)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
	}
	else if (mode == COMMAND)
	{
		signal(SIGINT, ft_sig_int);
		signal(SIGQUIT, ft_sig_quit);
		signal(SIGTSTP, SIG_IGN);
	}
}

//checks the exit status of process(if interrupted by signal)
//returns the error message
void	check_exit_status(void)
{
	if (g_exit_st == (128 | SIGQUIT))
		ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
	else if (g_exit_st == (128 | SIGSEGV))
		ft_putstr_fd("Segmentation fault (core dumped)\n", STDERR_FILENO);
	else if (g_exit_st == (128 | SIGINT))
		ft_putchar_fd('\n', STDERR_FILENO);
}
