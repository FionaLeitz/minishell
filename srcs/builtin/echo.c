/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 09:29:30 by fleitz            #+#    #+#             */
/*   Updated: 2022/08/15 14:07:26 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// check if -n applies
static int	ft_checkn(char **arg)
{
	int	i;
	int	count;

	count = 1;
	while (arg[count])
	{
		i = 0;
		if (arg[count][i] != '-')
			return (count);
		i++;
		while (arg[count][i])
		{
			if (arg[count][i] != 'n')
				return (count);
			i++;
		}
		count++;
	}
	return (count);
}

// write in stdout the arguments
int	ft_echo(char **arg)
{
	int	count;
	int	line;

	count = ft_checkn(arg);
	line = 0;
	if (count != 1)
		line++;
	while (arg[count] && arg[count + 1])
		ft_printf("%s ", arg[count++]);
	if (arg[count])
		ft_printf("%s", arg[count]);
	if (line != 1)
		ft_printf("\n");
	return (0);
}
