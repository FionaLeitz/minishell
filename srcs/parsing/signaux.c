/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 10:53:47 by masamoil          #+#    #+#             */
/*   Updated: 2022/08/23 17:08:29 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
		exit(g_exit_st);
	}
}
	
//handles the signals depending on mode
void	ft_signals(t_sig_mode mode)
{
	
	if (mode == DEFAULT)
	{
		signal(SIGINT, ft_sig_int);
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
	else if (mode == COMMAND)
	{
		signal(SIGINT, ft_sig_int);
		signal(SIGQUIT, ft_sig_quit);
	}
}

// handles ctrl-d
void	ft_exit_d(t_data *data, t_params *params)
{
	write(1, "exit\n", 4);
	write(1, "\n", 1);
	free_params(params);
	free_struct(data);
	exit(0);
}

//checks the exit status of process, returns the error message
void    check_exit_status(void)
{
    	if (WIFSIGNALED(g_exit_st) && WTERMSIG(g_exit_st) == 3)
	{
        	ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		g_exit_st = 131;
	}
    ///else if (exit_st == (128 | SIGSEGV))
       // ft_putstr_fd("Segmentation fault (core dumped)\n",
         //   STDERR_FILENO);
    	else if (WIFSIGNALED(g_exit_st) && WTERMSIG(g_exit_st) == 2)
	{
        	ft_putchar_fd('\n', STDERR_FILENO);
		g_exit_st = 130;
	}
}
