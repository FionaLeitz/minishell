/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masamoil <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 10:53:47 by masamoil          #+#    #+#             */
/*   Updated: 2022/07/11 10:36:34 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// handle ctrl-\ and ctrl-c
void	sig_manage(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}		
	if (signal == SIGQUIT)
		printf("\b\b  \b\b");
	return ;
}

// handle ctrl-d
void	ft_exit_d(t_data *data, t_params *params)
{
	printf("exit\n");
	free_params(params);
	free_struct(data);
	exit(0);
}

/*void	ft_sig_heredoc()
{

}*/
