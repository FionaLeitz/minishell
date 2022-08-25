/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fleitz <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 14:41:15 by fleitz            #+#    #+#             */
/*   Updated: 2022/08/15 14:12:03 by masamoil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// unset variable in export
static void	ft_unset_export(char **arg, t_params *params)
{
	int			i;
	int			j;
	t_export	*tmp;

	i = 1;
	while (arg[i])
	{
		tmp = params->export;
		while (tmp)
		{
			j = 0;
			while (arg[i][j] == tmp->name[j] && arg[i][j] != '\0')
				j++;
			if (arg[i][j] == '\0' && tmp->name[j] == '\0')
			{
				tmp->name[0] = '\0';
				tmp->value[0] = '\0';
				break ;
			}
			tmp = tmp->next;
		}
		i++;
	}
}

// unset variable in env
int	ft_unset(char **arg, t_params *params)
{
	int			i;
	int			j;
	int			count;

	i = 0;
	count = 0;
	while (arg[++i])
	{
		count = -1;
		while (params->env[++count])
		{
			j = -1;
			while (arg[i][++j])
				if (arg[i][j] != params->env[count][j])
					break ;
			if (arg[i][j] == '\0' && params->env[count][j] == '=')
			{
				params->env[count][0] = '\0';
				break ;
			}
		}
	}
	ft_unset_export(arg, params);
	return (0);
}
