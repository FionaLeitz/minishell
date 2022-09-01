/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 14:41:48 by fleitz            #+#    #+#             */
/*   Updated: 2022/08/15 14:44:38 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// check if arguments are numbers
static int	in_exit(char **arg)
{
	int	i;

	i = -1;
	while (arg[1][++i])
	{
		if (ft_isdigit(arg[1][i]) == 0)
		{
			ft_printf("minishell: exit: %s: numeric argument required\n",
				arg[1]);
			g_exit_st = 2;
			exit(2);
		}
	}
	return (0);
}

// exit with error messages
int	ft_exit(char **arg, int i)
{
	long int	n;

	if (i == 0)
		ft_printf("exit\n");
	n = 0;
	if (arg[1])
	{
		in_exit(arg);
		n = ft_atol(arg[1]);
	}
	if (arg[1] != NULL && arg[2] != NULL)
	{
		ft_printf("minisehll: exit: too many arguments\n");
		g_exit_st = 1;
		return (1);
	}
	g_exit_st = n;
	exit(g_exit_st);
}
