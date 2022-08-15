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

// exit with error messages
int	ft_exit(char **arg)
{
	long int	n;
	int			i;

	ft_printf("exit\n");
	n = 0;
	if (arg[1])
	{
		i = -1;
		while (arg[1][++i])
		{
			if (ft_isdigit(arg[1][i]) == 0)
			{
				ft_printf("minishell: exit: %s: numeric argument required\n",
					arg[1]);
				//exit_st = 2;?
				return (2);
			}
		}
		n = ft_atol(arg[1]);
	}
	if (arg[1] != NULL && arg[2] != NULL)
	{
		ft_printf("minisehll: exit: too many arguments\n");
		exit_st = 1;
		return (1);
	}
	return (n);
}
