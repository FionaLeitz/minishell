/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 14:41:48 by fleitz            #+#    #+#             */
/*   Updated: 2022/05/11 14:41:49 by fleitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
				return (2);
			}
		}
		n = ft_atol(arg[1]);
	}
	if (arg[1] != NULL && arg[2] != NULL)
	{
		ft_printf("minisehll: exit: too many arguments\n");		// should not exit if too many arguments
		return (1);
	}
	return (n);
}
